from queue import Empty
from subprocess import Popen, PIPE
from threading import Thread
import time
import psutil
import re


class LocalExecutor:
    def __init__(self, task_queue, thread_count):
        self.task_queue = task_queue
        self.thread_count = thread_count
        self.workers = None
        self.initialize_workers()

    def initialize_workers(self):
        self.workers = [LocalWorker(self.task_queue)
                        for _ in range(self.thread_count)]

    def start(self):
        for worker in self.workers:
            worker.start()

    def wait(self):
        for worker in self.workers:
            worker.join()


class LocalWorker(Thread):
    def __init__(self, task_queue):
        super(LocalWorker, self).__init__()
        self.task_queue = task_queue

    def run(self):
        while True:
            try:
                internal_task = self.task_queue.get(block=False)
                execute_task(internal_task)
            except Empty:
                break


def execute_task(internal_task):
    task = internal_task.task
    internal_task.future.set_running_or_notify_cancel()
    task_memory_limit = 7 * 1024 * 1024 * 1024  # 7 GB

    if task.memory_limit is not None:
        task_memory_limit = task.memory_limit * 1024 * 1024 * 1024

    run_task(internal_task, internal_task.task, task_memory_limit)


def get_mem_use(pid):

    _private_re = re.compile(br"\nPrivate.*:\s+(\d+)")
    _pss_re = re.compile(br"\nPss\:\s+(\d+)")
    _swap_re = re.compile(br"\nSwap\:\s+(\d+)")

    with open("%s/%s/smaps" % ('/proc', pid), "rb", buffering=8192) as f:
        smaps_data = f.read().strip()

        uss = sum(map(int, _private_re.findall(smaps_data))) * 1024
        # pss = sum(map(int, _pss_re.findall(smaps_data))) * 1024
        # swap = sum(map(int, _swap_re.findall(smaps_data))) * 1024

        # return uss + pss + swap
        return uss


def run_task(internal_task, task, task_memory_limit):
    try:
        start = time.time()

        process = Popen("exec " + task.command, stdin=PIPE,
                        stdout=PIPE, stderr=PIPE, shell=True)

        pid = process.pid

        if task.input is not None:
            process.stdin.write(task.input)

        while process.poll() is None:
            mem_used = psutil.virtual_memory().used # kill based on total server mem use
            # mem_used = get_mem_use(pid) # kill based on process mem use. Slow caused by repetive smaps reads.
            if mem_used >= task_memory_limit:
                internal_task.task.output = "OutOfMemory exceeded max_bytes: " + \
                    str(mem_used)
                process.terminate()
                process.kill()
            if (time.time() - start) > task.time_limit:
                internal_task.task.output = "OutOfTime exceeded time_limit: " + \
                    str(task.time_limit)
                process.terminate()
                process.kill()

            time.sleep(0.05)

        process.wait()

        if process.returncode == 0:
            task.output = b''.join(
                process.stdout.readlines())  # .decode('utf-8')

        task.error = b''.join(process.stderr.readlines())  # .decode('utf-8')

        internal_task.future.set_result(task)

        process.stdin.close()
        process.stdout.close()
        process.stderr.close()
    except Exception as e:
        internal_task.future.set_exception(e)
