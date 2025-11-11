import csv
from pathlib import Path

import matplotlib.pyplot as plt


def load_runtimes(csv_path: Path):
    runs = []
    durations = []
    with csv_path.open(newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            runs.append(int(row["runs"]))
            durations.append(int(row["ms"]))
    return runs, durations


def plot_runtimes(runs, durations):
    plt.figure(figsize=(8, 5))
    plt.plot(runs, durations, marker="o", linestyle="-", color="tab:blue")
    plt.title("Sensor Insert Runtime")
    plt.xlabel("Number of insertions")
    plt.ylabel("Runtime (ms)")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    csv_file = Path(__file__).parent / "runtimes.csv"
    if not csv_file.exists():
        raise FileNotFoundError(f"Could not find {csv_file}")
    runs_data, durations_data = load_runtimes(csv_file)
    plot_runtimes(runs_data, durations_data)

