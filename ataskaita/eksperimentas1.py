import subprocess
import matplotlib.pyplot as plt
import numpy as np

probs = np.arange(0, 1.01, 0.05)
num_of_seeds = 1000
a11_cmd = "./a11"
n = 7
k = 4
input_vec = "1111"
assert len(input_vec) == k
g = "1000101,0100110,0010111,0001011"
scenario = 1

results = {}

for p in probs:
    success_count = 0
    for seed in range(1, num_of_seeds + 1):
        cmd = f"echo {input_vec} | {a11_cmd} -p {p} -n {n} -k {k} -g {g} -s {scenario} --seed {seed}"
        process = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        output = process.stdout

        initial = None
        decoded = None
        for line in output.splitlines():
            if line.startswith("Initial vector:"):
                initial = line.split(":")[1].strip()
            if line.startswith("Decoded vector:"):
                decoded = line.split(":")[1].strip()
        if initial is not None and decoded is not None:
            if initial == decoded:
                success_count += 1
    results[p] = success_count / num_of_seeds

success_rates = [results[p] for p in probs]

plt.plot(probs, success_rates)
plt.title("Sėkmingo dekodavimo tikimybės priklausomybė nuo klaidos tikimybės")
plt.grid(True, which="both", ls="--")

plt.xlabel(r"Klaidos tikimybė ($p_e$)")
plt.xticks(np.arange(0, 1.01, 0.1))
plt.xlim(0, 1)

plt.ylabel("Dekodavimo sėkmės tikimybė")
plt.yticks(np.arange(0, 1.01, 0.1))
plt.ylim(bottom=0)

plt.savefig("grafikas1.svg")
plt.show()
