import matplotlib.pyplot as plt
import csv

# Read data from CSV file
sensor_counts = []
latencies = []

with open('task3.csv', 'r') as file:
    reader = csv.DictReader(file)
    for row in reader:
        sensor_counts.append(int(row['Sensor Count']))
        latencies.append(int(row['Latency (ms)']))

# Create the plot
plt.figure(figsize=(10, 6))
plt.plot(sensor_counts, latencies, marker='o', linewidth=2, markersize=8, color='#2E86AB')

# Customize the plot
plt.xlabel('Sensor Count', fontsize=12, fontweight='bold')
plt.ylabel('Latency (ms)', fontsize=12, fontweight='bold')
plt.title('Sensor Count vs Latency', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3, linestyle='--')
plt.tight_layout()

# Add value labels on each point
for i, (x, y) in enumerate(zip(sensor_counts, latencies)):
    plt.annotate(f'{y} ms', (x, y), textcoords="offset points", 
                 xytext=(0,10), ha='center', fontsize=9)

# Save the plot as an image file
plt.savefig('task3_plot.png', dpi=300, bbox_inches='tight')
print("Plot saved as task3_plot.png")

# Show the plot
plt.show()

