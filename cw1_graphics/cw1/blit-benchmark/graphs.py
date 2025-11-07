import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

# Placeholder variables - change these for different CSV files
FILE_NAME = 'clip_scenario_vs_performance.csv'  # Change to: clip_scenario_vs_performance.csv or image_size_vs_performance.csv
X_COLUMN_NAME = 'Clip Scenario'  # Change to: 'Clip Scenario' or 'Image Size (pixels)'
Y_COLUMN_NAME = 'CPU Time (ns)'
ALGORITHM_COLUMN = 'Algorithm'
GRAPH_TITLE = 'Clip Scenario vs Performance by Algorithm'  # Change title as needed
OUTPUT_FILENAME = 'clip_scenario_vs_performance.png'  # Change output filename as needed

def create_algorithm_comparison_graph(file_path, x_column, y_column, algorithm_column, title, output_file):
    """
    Reads CSV data, separates by algorithm, and plots line graphs for each algorithm.
    
    Args:
        file_path: Path to CSV file
        x_column: Name of X-axis column
        y_column: Name of Y-axis column
        algorithm_column: Name of algorithm column
        title: Graph title
        output_file: Output PNG filename
    """
    try:
        df = pd.read_csv(file_path)
    except (FileNotFoundError, pd.errors.EmptyDataError) as e:
        print(f"Error reading file: {e}")
        return
    
    if x_column not in df.columns or y_column not in df.columns or algorithm_column not in df.columns:
        print(f"Error: CSV must contain columns '{x_column}', '{y_column}', and '{algorithm_column}'.")
        return

    # Get unique algorithms
    algorithms = df[algorithm_column].unique()
    
    # Define colors for each algorithm
    algorithm_colors = {
        'blit_masked': '#D93A00',      # Red
        'blit_ex_solid': '#0066CC',    # Blue
        'blit_ex_memcpy': '#00AA00'    # Green
    }
    
    # Create figure
    plt.figure(figsize=(12, 8))
    
    # Map clip scenarios to percentages if needed
    clip_scenario_to_percentage = {
        'Center (0)': '100%',
        'Light (1)': '75%',
        'Partial (2)': '50%',
        'Heavy (3)': '25%',
        'Very Heavy (4)': '10%'
    }
    
    # Create a display column for X-axis labels
    if x_column == 'Clip Scenario':
        df['x_display'] = df[x_column].map(clip_scenario_to_percentage)
    else:
        df['x_display'] = df[x_column]
    
    # Plot a line for each algorithm
    for algorithm in algorithms:
        # Filter data for this algorithm
        algo_data = df[df[algorithm_column] == algorithm].copy()
        
        # Sort by X values for proper line plotting
        # For resolution, we need to handle "1920x1080" format specially
        if x_column == 'Resolution':
            # Create a sortable column
            algo_data['sort_key'] = algo_data[x_column].str.split('x').str[0].astype(int)
            algo_data = algo_data.sort_values(by='sort_key', ascending=True)
        elif x_column == 'Clip Scenario':
            # Extract number from "Center (0)" format for sorting
            algo_data['sort_key'] = algo_data[x_column].str.extract(r'\((\d+)\)').astype(int)
            algo_data = algo_data.sort_values(by='sort_key', ascending=False)  # Descending: 100% to 10%
        elif x_column == 'Image Size (pixels)':
            # Extract first number from "256x256" format
            algo_data['sort_key'] = algo_data[x_column].str.split('x').str[0].astype(int)
            algo_data = algo_data.sort_values(by='sort_key', ascending=True)
        else:
            # Default: try to sort by X column directly
            algo_data = algo_data.sort_values(by=x_column, ascending=True)
        
        # Get color for this algorithm
        color = algorithm_colors.get(algorithm, '#000000')
        
        # Plot line with markers using display labels
        plt.plot(algo_data['x_display'], algo_data[y_column], 
                marker='o', 
                markersize=8,
                linewidth=2,
                color=color,
                label=algorithm,
                alpha=0.8)
        
        # Add data labels
        for index, row in algo_data.iterrows():
            plt.text(row['x_display'], 
                    row[y_column] * 1.05,
                    f'{int(row[y_column]):,}', 
                    ha='center', va='bottom', fontsize=9, color=color)
    
    # Apply styling
    plt.title(title, fontsize=18, fontweight='bold')
    # Use display-friendly label for X-axis
    if x_column == 'Clip Scenario':
        plt.xlabel('Visibility (%)', fontsize=14)
    else:
        plt.xlabel(x_column, fontsize=14)
    plt.ylabel(y_column, fontsize=14)
    
    plt.grid(axis='both', linestyle=':', alpha=0.7)
    
    plt.gca().spines['top'].set_visible(False)
    plt.gca().spines['right'].set_visible(False)
    
    # Format Y-axis to show full numbers with commas
    def format_with_commas(value, pos):
        return f'{int(value):,}'
    
    plt.gca().yaxis.set_major_formatter(FuncFormatter(format_with_commas))
    
    # Add legend
    plt.legend(frameon=True, fontsize=12, loc='best')
    
    # Set x-axis ticks explicitly for clip scenarios to ensure correct order
    if x_column == 'Clip Scenario':
        plt.xticks(['100%', '75%', '50%', '25%', '10%'], rotation=45, ha='right', fontsize=12)
    else:
        plt.xticks(rotation=45, ha='right', fontsize=12)
    
    plt.tight_layout()
    
    plt.savefig(output_file, dpi=300)
    
    print(f"\nSuccess! Graph saved as: {output_file}")

if __name__ == "__main__":
    create_algorithm_comparison_graph(
        FILE_NAME, 
        X_COLUMN_NAME, 
        Y_COLUMN_NAME, 
        ALGORITHM_COLUMN,
        GRAPH_TITLE,
        OUTPUT_FILENAME
    )

