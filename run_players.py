import signal
import subprocess
import threading
from random import randint
from sys import argv
from concurrent.futures import ThreadPoolExecutor

processes = []


def handle_signal(*_):
    print("Received termination signal. Stopping...")
    # Send SIGINT to all subprocesses
    for process in processes:
        process.send_signal(signal.SIGINT)
        processes.remove(process)


def read_output(process: subprocess.Popen[str], number: int):
    # This function reads the stdout of the subprocess in a separate thread
    for line in iter(process.stdout.readline, b''):
        line = line.strip()
        if line:
            print(f'{number}: {line}')


def run_player(number: int):
    team_id = randint(1, 10)

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
    threading.Thread(target=read_output, args=(process, number), daemon=True).start()

    # Wait for the process to complete
    process.wait()


def run_display():
    process = subprocess.Popen('./lemipc_display', shell=False)
    processes.append(process)

def main():
    if len(argv) < 2 or not argv[1].isdigit():
        print("Usage: python run_players.py <number of players>")
        exit(1)

    n = int(argv[1])

    signal.signal(signal.SIGINT, handle_signal)

    # Use ThreadPoolExecutor to run commands in parallel
    with ThreadPoolExecutor() as executor:
        # Map each command to the executor
        executor.submit(run_display)
        executor.map(run_player, range(1, n + 1))
        executor.shutdown(wait=True)


if __name__ == '__main__':
    main()
