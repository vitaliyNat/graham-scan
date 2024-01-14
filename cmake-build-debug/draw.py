import turtle

file_path = 'hull.txt'
try:
    with open(file_path, 'r') as file:

        hull= eval(file.read())
except FileNotFoundError:
    print(f"The file '{file_path}' does not exist.")
except IOError as e:
    print(f"An error occurred while reading the file: {e}")

file_path = 'surface.txt'
try:
    with open(file_path, 'r') as file:
        Surface= eval(file.read())
except FileNotFoundError:
    print(f"The file '{file_path}' does not exist.")
except IOError as e:
    print(f"An error occurred while reading the file: {e}")


screen = turtle.Screen()
pen = turtle.Turtle()

pen.speed(1000)
def draw_point(x, y, color):
    pen.penup()
    pen.goto(x*300 , y*300)
    pen.pendown()
    pen.dot(5, color)

for point in Surface:
    draw_point(*point, 'blue')

pen.penup()
pen.pencolor('red')
pen.goto(hull[0][0]*300,hull[0][1]*300)
pen.pendown()
for point in hull:
    pen.goto(point[0]*300,point[1]*300)
pen.goto(hull[0][0]*300,hull[0][1]*300)
pen.penup()

screen.exitonclick()
