import heapq

# Global variable for fringe entries
fringe_entries = {}

# Heuristic Function
def heuristicFn(state, goal):
    # Use Manhattan distance as the heuristic
    return abs(state[0] - goal[0]) + abs(state[1] - goal[1])

# Fringe list manager (priority queue with heapq)
def addToFringe(fringe, new_entry):
    f_cost, g_cost, state, path, prev_direction = new_entry

    if state not in fringe_entries:
        fringe_entries[state] = (f_cost, g_cost)
        heapq.heappush(fringe, new_entry)
    else:
        # Only update if the new g_cost is less than the existing one
        if g_cost < fringe_entries[state][1]:  # Compare g_cost
            # Remove the old entry from the fringe if it exists
            # Create a new list to avoid modifying the heap directly
            fringe = [(fc, gc, st, p, pd) for (fc, gc, st, p, pd) in fringe if st != state]
            fringe_entries[state] = (f_cost, g_cost)
            heapq.heappush(fringe, new_entry)

    return fringe  # Return the modified fringe for further use

# A* function: Selects the next node to expand based on the fringe
def nextPartialPlanAStar(fringe):
    while fringe:
        entry = heapq.heappop(fringe)
        f_cost, g_cost, current_state, path, prev_direction = entry
        return f_cost, g_cost, current_state, path, prev_direction  # Return the next node to expand (lowest cost)

    return None  # If the fringe is empty, return None

# Search function
def search(start, goal, barriers):
    fringe = []
    expanded = set()  # Keep track of expanded states

    # Initial state
    addToFringe(fringe, (heuristicFn(start, goal), 0, start, [start], None))

    while fringe:
        f_cost, g_cost, current_state, path, prev_direction = nextPartialPlanAStar(fringe)

        # Mark current state as expanded
        expanded.add(current_state)

        # Check if goal is reached
        if current_state == goal:
            return path, expanded, g_cost, f_cost

        # Expand neighbors (example, replace with your own logic)
        for neighbor in get_neighbors(current_state, barriers):
            new_g_cost = g_cost + 1  # Update cost as per your logic
            addToFringe(fringe, (new_g_cost + heuristicFn(neighbor, goal), new_g_cost, neighbor, path + [neighbor], current_state))

    return None, None, None, None  # Return None if no path is found