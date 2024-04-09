with open('SantaClaus.txt', 'r') as file:
    lines = file.readlines()

with open('SantaClaus_modified.txt', 'w') as file:
    for line in lines:
        file.write(line.strip() + '\\\\\n')
