#ifndef CONSTANT_H
#define CONSTANT_H

enum ObjectType{DEFAULT,FOOD,REWARD,WALL,SNAKE};
enum Type{GD_Type};
typedef enum{SINGLE,DOUBLE,AUTO} Mode;

//food.h
#define FUNCTION_TYPE 5
#define FOOD_SIZE 10
#define FOOD_RADIUS 4
//food.h
//gamecontroller.h
#define SPECIAL_INTERVAL 10000

#define REWARD_INTERVAL 10000

#define DEFAULT_PERIOD 100
//gamecontroller.h
//wall.h
#define DEFAULT_WALL_WIDTH 50

#define DEFAULT_WALL_HEIGHT 10

#define DEFAULT_WALL_COUNT 10
//wall.h
//snake.h
#define DEFAULT_LENGTH 5

#define SNAKE_SIZE 10

#define DEFAULT_COLOR Qt::red
//snake.h

#define HALF_PICTURE_WIDTH (300+10)

#define HALF_PICTURE_HEIGHT (200+10)

#define X_MIN (-HALF_PICTURE_WIDTH-SNAKE_SIZE)

#define X_MAX (HALF_PICTURE_WIDTH-SNAKE_SIZE)

#define Y_MIN (-HALF_PICTURE_HEIGHT-SNAKE_SIZE)

#define Y_MAX (HALF_PICTURE_HEIGHT-SNAKE_SIZE)

#define X_RANGE (X_MAX-X_MIN)

#define Y_RANGE (Y_MAX-Y_MIN)

#endif // CONSTANT_H
