import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

def create_line_algorithm_graph(file_path, title, output_file):
    """
    Reads CSV data, separates by algorithm, and plots line graphs for each algorithm.
    
    Args:
        file_path: Path to CSV file
        title: Graph title
        output_file: Output PNG filename
    """
    try:
        df = pd.read_csv(file_path)
    except (FileNotFoundError, pd.errors.EmptyDataError) as e:
        print(f"Error reading file: {e}")
        return
    
    # Check required columns
    required_columns = ['CPU Time (ns)', 'Line Length (pixels)', 'Algorithm']
    if not all(col in df.columns for col in required_columns):
        print(f"Error: CSV must contain columns: {required_columns}")
        return

    # Convert CPU Time to numeric, handling any non-numeric values (like "CRASH")
    df['CPU Time (ns)'] = pd.to_numeric(df['CPU Time (ns)'], errors='coerce')
    
    # Remove rows with invalid CPU times (NaN from conversion errors)
    df = df.dropna(subset=['CPU Time (ns)'])
    
    # Filter out crash points (0 or very large values like 999999999, or string "CRASH")
    # We'll mark them separately if needed
    crash_mask = (df['CPU Time (ns)'] == 0) | (df['CPU Time (ns)'] > 1000000000)
    crash_data = df[crash_mask].copy()
    df = df[~crash_mask].copy()
    
    # Get unique algorithms
    algorithms = df['Algorithm'].unique()
    
    # Define colors for each algorithm
    algorithm_colors = {
        'DDA': '#D93A00',           # Red
        'Bresenham': '#0066CC',     # Blue
        'Baseline': '#00AA00'       # Green
    }
    
    # Create figure
    plt.figure(figsize=(12, 8))
    
    # Plot each algorithm
    for algorithm in algorithms:
        alg_data = df[df['Algorithm'] == algorithm].copy()
        
        # Sort by line length for proper line plotting
        alg_data = alg_data.sort_values(by='Line Length (pixels)')
        
        # Get color for this algorithm
        color = algorithm_colors.get(algorithm, '#000000')
        
        # Plot the line
        plt.plot(alg_data['Line Length (pixels)'], 
                alg_data['CPU Time (ns)'],
                marker='o',
                linewidth=2,
                markersize=8,
                label=algorithm,
                color=color)
        
        # Add data labels
        for _, row in alg_data.iterrows():
            plt.text(row['Line Length (pixels)'], 
                    row['CPU Time (ns)'] * 1.02,
                    f'{int(row["CPU Time (ns)"]):,}',
                    ha='center',
                    va='bottom',
                    fontsize=9,
                    color=color)
    
    # Mark crash points if any
    crash_labels_added = set()  # Track which crash labels we've already added
    if not crash_data.empty:
        for algorithm in crash_data['Algorithm'].unique():
            crash_alg = crash_data[crash_data['Algorithm'] == algorithm]
            color = algorithm_colors.get(algorithm, '#000000')
            for _, row in crash_alg.iterrows():
                # Only add label if we haven't added one for this algorithm yet
                crash_label = f'{algorithm} (Crashed)'
                use_label = crash_label if crash_label not in crash_labels_added else ''
                if use_label:
                    crash_labels_added.add(crash_label)
                
                plt.scatter(row['Line Length (pixels)'],
                           plt.ylim()[1] * 0.95,  # Place near top of graph
                           marker='X',
                           s=200,
                           color=color,
                           edgecolors='black',
                           linewidths=2,
                           zorder=10,
                           label=use_label)
                plt.text(row['Line Length (pixels)'],
                        plt.ylim()[1] * 0.98,
                        'CRASH',
                        ha='center',
                        va='bottom',
                        fontsize=10,
                        fontweight='bold',
                        color='red',
                        bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.7))
    
    # Styling
    plt.title(title, fontsize=18, fontweight='bold')
    plt.xlabel('Line Length (pixels)', fontsize=14)
    plt.ylabel('CPU Time (ns)', fontsize=14)
    
    plt.grid(axis='both', linestyle=':', alpha=0.7)
    plt.gca().spines['top'].set_visible(False)
    plt.gca().spines['right'].set_visible(False)
    
    # Format Y-axis to show full numbers with commas
    def format_with_commas(value, pos):
        return f'{int(value):,}'
    
    plt.gca().yaxis.set_major_formatter(FuncFormatter(format_with_commas))
    
    # Format X-axis
    plt.gca().xaxis.set_major_formatter(FuncFormatter(format_with_commas))
    
    # Add legend
    plt.legend(frameon=True, fontsize=12, loc='best')
    
    plt.tight_layout()
    
    # Save the graph
    plt.savefig(output_file, dpi=300)
    print(f"\nSuccess! Graph saved as: {output_file}")
    plt.close()

def main():
    # Generate graph for horizontal lines
    create_line_algorithm_graph(
        'horizontal_lines_vs_performance.csv',
        'Horizontal Lines: CPU Time vs Line Length by Algorithm',
        'horizontal_lines_vs_performance.png'
    )
    
    # Generate graph for diagonal lines
    create_line_algorithm_graph(
        'diagonal_lines_vs_performance.csv',
        'Diagonal Lines: CPU Time vs Line Length by Algorithm',
        'diagonal_lines_vs_performance.png'
    )
    
    print("\nAll graphs generated successfully!")

if __name__ == "__main__":
    main()

