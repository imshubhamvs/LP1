import threading
import time
import random

class LamportClock:
    def __init__(self):
        self.counter = 0
        self.lock = threading.Lock()

    def tick(self):
        with self.lock:
            self.counter += 1

    def receive_event(self, received_counter):
        with self.lock:
            self.counter = max(self.counter, received_counter) + 1

    def get_counter(self):
        with self.lock:
            return self.counter


class Process(threading.Thread):
    def __init__(self, id, clock, all_processes):
        super().__init__()
        self.id = id
        self.clock = clock
        self.all_processes = all_processes

    def run(self):
        for _ in range(5):  # Each process will perform 5 events
            time.sleep(random.uniform(0.5, 1.5))  # Simulate some processing time
            self.send_event()
            self.synchronize_clocks()

    def send_event(self):
        self.clock.tick()
        print(f"Process {self.id} sending event with timestamp: {self.clock.get_counter()}")

    def synchronize_clocks(self):
        current_time = self.clock.get_counter()
        print(f"Process {self.id} synchronizing clocks with value: {current_time}")

        for process in self.all_processes:
            if process.id != self.id:
                process.clock.receive_event(current_time)
                print(f"Process {self.id} updated {process.id}'s clock to: {process.clock.get_counter()}")


def main():
    num_processes = 3
    clocks = [LamportClock() for _ in range(num_processes)]
    processes = [Process(i, clocks[i], clocks) for i in range(num_processes)]

    for process in processes:
        process.start()

    for process in processes:
        process.join()


if __name__ == "__main__":
    main()
