import threading
import random
import time

BUFFER_SIZE = 5
MAX_ITERATIONS = 20
buffer = [0] * BUFFER_SIZE
in_index = 0
out_index = 0
total_produced = 0
total_consumed = 0
empty_semaphore = threading.Semaphore(BUFFER_SIZE)
full_semaphore = threading.Semaphore(0)
mutex = threading.Semaphore(1)

def print_buffer():
    print("[", end="")
    for item in buffer:
        print(item, end=" ")
    print("]")

def producer():
    global in_index, total_produced
    while total_produced < MAX_ITERATIONS:
        item = random.randint(0, 19)
        empty_semaphore.acquire()
        mutex.acquire()

        buffer[in_index] = item
        print(f"Produced {item} at position {in_index}")
        print_buffer()

        in_index = (in_index + 1) % BUFFER_SIZE
        total_produced += 1

        mutex.release()
        full_semaphore.release()
        time.sleep(1)
    print("Producer finished")

def consumer():
    global out_index, total_consumed
    while total_consumed < MAX_ITERATIONS:
        full_semaphore.acquire()
        mutex.acquire()

        item = buffer[out_index]
        print(f"Consumed {item} from position {out_index}")
        buffer[out_index] = 0
        print_buffer()

        out_index = (out_index + 1) % BUFFER_SIZE
        total_consumed += 1

        mutex.release()
        empty_semaphore.release()
        time.sleep(2)
    print("Consumer has finished consuming")

if __name__ == "__main__":
    producer_thread = threading.Thread(target=producer)
    consumer_thread = threading.Thread(target=consumer)

    producer_thread.start()
    consumer_thread.start()

    producer_thread.join()
    consumer_thread.join()
