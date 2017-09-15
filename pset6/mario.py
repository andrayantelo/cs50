#http://docs.cs50.net/problems/mario/more/mario.html
def build_pyramid(h):
    hash = '#'
    for i in range(1,h + 1):
        print(" "*(h - i), end="")
        print(hash*i, end="")
        print(" "*2, end="")
        print(hash*i)

if __name__ == "__main__":
    height = int(input("Height: "))
    while ((height < 0) or (height > 23)):
        height = input("Height: ")
    build_pyramid(height)