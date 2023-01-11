def tile(n):
    return pow(2, n) * (n - 1) - 4


board_size = int(input("Enter board size: "))
num_of_tiles = pow(board_size, 2)
score = 0

for i in range(3, num_of_tiles + 2):
    score += tile(i)

largest_tile = pow(2, num_of_tiles + 1)
print("Largest tile:", largest_tile)
print("Score:", score)

