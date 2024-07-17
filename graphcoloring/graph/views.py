import subprocess
import json
import os
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import render
 
@csrf_exempt
def graph_view(request):
    if request.method == 'GET':
        return render(request, 'graph.html')
    elif request.method == 'POST':
        adj_data = request.POST.get('adj_data', '')
        adj_list = json.loads(adj_data)
 
        # Convert adjacency list to edge list and write to input.txt
        edge_set = set()
        nodes = set()
        n=0
        for node, neighbors in adj_list.items():
            n += 1
            nodes.add(int(node))
            for neighbor in neighbors:
                edge = tuple(sorted((int(node), int(neighbor))))
                if edge not in edge_set:
                    edge_set.add(edge)
                    nodes.add(int(neighbor))

        # Convert edge_set to a list if needed
        edge_list = list(edge_set)
        m = len(edge_list)
        input_path = "D:\Graph Coloring App\graphcoloring\graphcoloring\input.txt"
        output_path = "D:\Graph Coloring App\graphcoloring\graphcoloring\output.txt"
        with open(input_path, 'w') as f:
            f.write(f"{n} {m}\n")
            for u, v in edge_list:
                f.write(f"{u} {v}\n")
        # Ensure the output file does not already exist
        if os.path.exists(output_path):
            os.remove(output_path)
        # Call the C++ program
        try:
            # Compile the C++ program
            subprocess.run(['g++', '-o', 'graph_coloring.exe', 'D:\Graph Coloring App\graphcoloring\graphcoloring\graphcoloring4.cpp'], check=True)
            # Execute the compiled C++ program
            result = subprocess.run(["graph_coloring.exe", input_path, output_path], capture_output=True, text=True, check=True)
            print('C++ Program Output:', result.stdout)  # Debugging line
        except subprocess.CalledProcessError as e:
            return JsonResponse({'error': 'C++ program failed to run', 'details': str(e), 'stdout': e.stdout, 'stderr': e.stderr})
 
        # Read the output from the C++ program
        try:
            with open(output_path, 'r') as f:
                chromatic_number = int(f.readline().strip())
                colors = {}
                for i, line in enumerate(f):
                    colors[str(i)] = int(line.strip())
        except Exception as e:
            return JsonResponse({'error': 'Failed to read output file', 'details': str(e)})
        return JsonResponse({'chromatic_number': chromatic_number, 'colors': colors})
    