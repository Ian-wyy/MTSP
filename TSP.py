import didppy as dp

def read_tsptw_data(filename):
    # Read file
    with open(filename, 'r') as file:
        lines = file.readlines()

    # Read the data
    num_locations = int(lines[0].strip())  # The number of point
    travel_time = []
    for i in range(1, num_locations + 1):
        line = lines[i].strip().split()
        travel_time.append([float(x) for x in line])  # Map

    ready_time = []
    due_time = []
    for i in range(num_locations + 1, 2 * num_locations + 1):
        line = lines[i].strip().split()
        ready_time.append(int(line[0]))  # Ready time
        due_time.append(int(line[1]))  # Due time

    return num_locations, travel_time, ready_time, due_time

# Use function to read data
""" num_locations, travel_time, ready_time, due_time = read_tsptw_data('SolomonPotvinBengio/rc_201.1.txt') """
num_locations, travel_time, ready_time, due_time = read_tsptw_data('sample.txt')

# Use didppy model
model = dp.Model(maximize=False, float_cost=True)
customer = model.add_object_type(number=num_locations)

# Define state variable
unvisited = model.add_set_var(object_type=customer, target=list(range(1, num_locations)))
location = model.add_element_var(object_type=customer, target=0)
time = model.add_float_resource_var(target=0, less_is_better=True)

# Add table of time window and travel time
ready_time_table = model.add_float_table(ready_time)
due_time_table = model.add_float_table(due_time)
travel_time_table = model.add_float_table(travel_time)

# Define state transition function
for j in range(1, num_locations):
    visit = dp.Transition(
        name="visit {}".format(j),
        cost=travel_time_table[location, j] + dp.FloatExpr.state_cost(),
        preconditions=[
            unvisited.contains(j),
            time + travel_time_table[location, j] <= due_time_table[j]
        ],
        effects=[
            (unvisited, unvisited.remove(j)),
            (location, j),
            (time, dp.max(time + travel_time_table[location, j], ready_time_table[j]))
        ],
    )
    model.add_transition(visit)

# Return the transition function of state
return_to_depot = dp.Transition(
    name="return",
    cost=travel_time_table[location, 0] + dp.FloatExpr.state_cost(),
    effects=[
        (location, 0),
        (time, time + travel_time_table[location, 0]),
    ],
    preconditions=[unvisited.is_empty(), location != 0]
)
model.add_transition(return_to_depot)

# Basic situation
model.add_base_case([unvisited.is_empty(), location == 0])

# Solve the model
solver = dp.CABS(model)
solution = solver.search()

# Output Solutoin
print("Transitions to apply:")
for t in solution.transitions:
    print(t.name)

print("Cost:", solution.cost)