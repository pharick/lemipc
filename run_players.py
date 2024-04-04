import signal
import subprocess
import threading
from time import sleep
from sys import argv
from concurrent.futures import ThreadPoolExecutor

processes = []
threads = []


def handle_signal(*_):
    print("Received termination signal. Stopping...")
    # Send SIGINT to all subprocesses
    for process in processes:
        process.send_signal(signal.SIGINT)
        processes.remove(process)
    # Wait for all threads to finish
    for thread in threads:
        thread.join()


def read_output(process: subprocess.Popen[str], number: int):
    # This function reads the stdout of the subprocess in a separate thread
    for line in iter(process.stdout.readline, b''):
        line = line.strip()
        if line:
            print(f'{number}: {line}')
        if process.poll() is not None:
            break


def run_player(number: int, team_id: int):
    # Start the subprocess and specify stdout as a pipe to capture it
    process = subprocess.Popen(
        f'./lemipc {team_id}',
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        shell=True,
        bufsize=1,
        universal_newlines=True,
    )

    processes.append(process)

    # Start a new thread to read the subprocess's stdout output
    thread = threading.Thread(target=read_output, args=(process, number), daemon=True)
    thread.start()
    threads.append(thread)

    # Wait for the process to complete
    process.wait()


def run_display():
    process = subprocess.Popen('./lemipc_display', shell=True)
    processes.append(process)

def main():
    if len(argv) < 2 or not all(x.isdigit() for x in argv[1:]):
        print("Usage: python run_players.py <team_count> [team_count...]")
        exit(1)
    if len(argv) - 1 > 10:
        print("Maximum 10 teams allowed")
        exit(1)

    team_counts = [int(x) for x in argv[1:]]
    print(team_counts)

    signal.signal(signal.SIGINT, handle_signal)

    # Use ThreadPoolExecutor to run commands in parallel
    with ThreadPoolExecutor(max_workers=sum(team_counts) + 1) as executor:
        # Map each command to the executor
        executor.submit(run_display)
        i = 1
        for team_id, team_count in enumerate(team_counts, start=1):
            for _ in range(team_count):
                executor.submit(run_player, i, team_id)
                if i == 1:
                    sleep(0.5)
                i += 1

        executor.shutdown(wait=True)


if __name__ == '__main__':
    main()
