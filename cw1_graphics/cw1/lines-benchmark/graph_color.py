import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

FILE_NAME = 'color_vs_performance.csv' # Assuming your new CSV is named this
X_COLUMN_NAME = 'Color'                  # Your color names are here
Y_COLUMN_NAME = 'CPU Time (ns)'          # Your performance metric

def create_color_performance_graph(file_path):
    try:
        df = pd.read_csv(file_path)
    except (FileNotFoundError, pd.errors.EmptyDataError) as e:
        print(f"Error reading file: {e}")
        return
    
    if X_COLUMN_NAME not in df.columns or Y_COLUMN_NAME not in df.columns:
        print(f"Error: CSV must contain columns '{X_COLUMN_NAME}' and '{Y_COLUMN_NAME}'.")
        return

    # Sort the data by CPU Time for better visual comparison (optional, but good for reports)
    df = df.sort_values(by=Y_COLUMN_NAME, ascending=True)

    plt.figure(figsize=(14, 8)) # Increased figure size for more bars

    # Get the list of colors and their corresponding CPU times
    colors_for_bars = df[X_COLUMN_NAME].tolist()
    cpu_times = df[Y_COLUMN_NAME].tolist()
    
    # --- Create the Bar Chart with Custom Colors ---
    # Matplotlib can directly use color names as arguments
    bars = plt.bar(colors_for_bars, cpu_times, color=colors_for_bars, edgecolor='black', linewidth=1.2)
    
    # Apply Professional Styling
    plt.title('CPU Time (ns) vs. Displayed Color', fontsize=18, fontweight='bold')
    plt.xlabel('Color Tested', fontsize=14)
    plt.ylabel(Y_COLUMN_NAME, fontsize=14)
    
    plt.grid(axis='y', linestyle=':', alpha=0.7) # Only Y-axis grid for bar charts
    
    plt.gca().spines['top'].set_visible(False)
    plt.gca().spines['right'].set_visible(False)
    
    # Add data labels on top of the bars
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width() / 2., 
                 height + (max(cpu_times) * 0.001), # Position slightly above the bar
                 f'{height:.0f}', # Format as integer for CPU time (assuming it's whole ns)
                 ha='center', va='bottom', fontsize=11, fontweight='bold')
    
    # Adjust Y-axis limit
    plt.ylim(min(cpu_times) * 0.99, max(cpu_times) * 1.01) # Zoom in on the relevant range

    # Rotate X-axis labels if needed for long color names
    plt.xticks(rotation=0, ha='center', fontsize=12) # No rotation if names are short
    plt.tight_layout()

    output_filename = 'color_performance_bar_chart.png'
    plt.savefig(output_filename, dpi=300)
    
    print(f"\nSuccess! Graph saved as: {output_filename}")


if __name__ == "__main__":
    create_color_performance_graph(FILE_NAME)