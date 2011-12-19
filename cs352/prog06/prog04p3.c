/*=============================================================================
| Assignment: Program 4, part 3 Magic Square
|
| Author: Brittany Latawiec
| Language: Written in C using GCC to complie
| To Compile: Go to directory containing file, type 'gcc prog04p3.c' and then 
| 'a.out' followed by the 3 integers to be passed in.
| Or 'gcc -o (someFileNameHere) prog04p3.c' and then
| 'someFileNameHere' followed by the 3 integers to be passed in. 
|
| Class: CSC 352 Fall 2011
| Instructor: Dr. McCann
| Due Date: 9/22/2011 5pm 
|
+-----------------------------------------------------------------------------
|
| Description: This program reads in from stdin 3 integers.
|
| Input: Three integers that represent the dimensions of the magic square
| and then where to place the number 1.
|
| Output: Magic square, magic constant, whether or not is a complete magic
| square.
|
| Algorithm: This program reads in the three integers using scanf(). After 
| reading in the integers they are checked to see if they are in
| the correct range (such as the row and column index do not
| exceed the bounds of the magic square and are not negative).
| An error message is shown if the arguments are out of range or
| if there isn't 3. If all arguments are alright, they are sent
| to the printMagicSquareAndDecideMagic() function. This function
| first creates a 2D array of ints that is the correct size. The 
| constant is then computed and stored in a variable. The array is 
| then initialized with all zeros to make it easier to work with 
| when placing real values. The 1 is then placed in its specified
| place so the filling of the array with values 2-(dimensions^2).
| There are 3 possible ways moving up and to the right can result
| in the position to place the new value being out of bounds. 
| The row can be -1 and the column in bounds, the row can be -1
| and the column > size, and the row can be in bounds but the
| column > size. Each of these cases is checked for and the 
| position modified as if the array was circular. There is also
| the case is the position is already occupied by a value. In this
| case a while loop is used to keep modifying the position for as
| long as needed to ensure a proper and empty position is found.
| This requires the storing of the last correctly placed row and
| column variables. Once all checks are completed and a proper 
| position is found, the value is placed. The array is then printed
| to the screen once all values are placed correctly along with
| the magic constant on the next line down. In order to determine
| whether or not the square is truly 'magic', the sum of each row,
| column, and both diagonals have to be checked. I use a for loop
| to check each row, another for loop to check each column, and two
| more for loops to check each diagonal. If the sum of any row,
| column, or diagonal turns out not to be equal to the magic constant
| a message stating which row, column, or diagonal was found that
| wasn't equal to the magic constant is printed to the screen and
| the program exits. If all rows, columns, and diagonals add up
| to the magic constant, a message stating so is printed to the
| screen.
|
| 
| Required Features Not Included: None.
| 
| Known Bugs: None. 
|
*===========================================================================*/
#include <stdio.h>
#include <stdlib.h>

void printSquareAndDecideMagic(int,int,int);

int main(void)
{

    int dimensions=-1, row=-1, col=-1, constant=0;
    
    
    scanf("%d %d %d", &dimensions, &row, &col);
    
    if( (dimensions < 3 || dimensions >31|| dimensions %2 ==0) || row <0 || row >= dimensions|| col < 0 || col >= dimensions){
        printf("Invalid or too few arguments!\n");
        exit(1);
    }
    
    printSquareAndDecideMagic(dimensions, row, col);
    
}


/*-----------------------------------------------------------------
| 
| Function printSquareAndDecideMagic
|
| 
| Purpose: This function takes in the dimensions, row, and col
| integers and creates a 2D array of the proper size
| and fills it with all the values ranging from 1 to
| dimensions^2. Once that is done correctly, the array
| is printed to the screen along with the magic constant.
| Then the sum of each row, column, and diagonal is 
| determined and compared to the magic constant. If any 
| are not equal to the magic constant then a message is 
| printed to the screen stating so and the program exits. 
| If all are equal, a message is printed to the screen 
| stating the square is magic.
| 
| 
| Parameters:
| 
| dimensions (IN) -- This is the passed in integer that is 
| read from scanF that determines the size of the magic
| square (2D array), which is (dimensions x dimensions). 
| It also determines what numbers to put in the square
| as well as the magic constant. 
| 
| row (IN) -- This is the passed in integer that is read in 
| from scanF that signifies the row at which to place 1 
| 
| 
| col (IN) -- This is the passed in integer that is read in 
| from scanF that signifies the column at which to place 1
| 
| 
| Returns: void
| 
*-------------------------------------------------------------------*/




void printSquareAndDecideMagic(int dimensions, int row, int col){

int squared=0, constant=0, i=0, j=0, k=0, increment=2, tempRow=row, tempCol=col, oldRow=row, oldCol=col, sum=0;

int square[dimensions][dimensions];

char message[100]="";

squared = dimensions * dimensions;

constant = (dimensions*(squared +1))/2;

/*For loop to fill the array initially with zeros to make checking
the values later easier*/

for(i=0; i< dimensions; i++){
for(j=0; j< dimensions; j++){
square[i][j]=0;
}
}

square[row][col]=1;

/*For loop to put fill magic square with numbers 2-dimensions^2. Here
there are several conditions that have to be met properly in order to 
ensure proper placement of the values. For one, if moving up and to the
right goes out of bounds on the 2D array, I wrap it around as if the array
was circular. Since there are several different ways this could happen
I have to have different if statements to cover each possibility. Also, if
there is already a value in the position the next value should go in then
the last correctly placed value needs to be referenced and the new value
placed below it. These conditions can coincide with each other, hence the
while loop. Once the array is properly filled, it is printed to the screen
along with the magic constant on the next line down.*/

for(k=1; k<squared; k++){

tempRow -=1;
tempCol +=1;

/*If place to move new value is out of bounds by the row only*/

if(tempRow < 0 && (tempCol >= 0 && tempCol < dimensions)){
tempRow = dimensions-1;
}

/*If place to move new value is out of bounds by the row and column*/

if(tempRow < 0 && tempCol >= dimensions){
tempRow = dimensions -1;
tempCol = 0;
}

/*If place to move new value is out of bounds by the column only*/

if(tempCol >= dimensions && (tempRow >= 0 && tempRow < dimensions)){
tempCol = 0;
}

/*While place to move new value has already had a value placed in it*/

while(square[tempRow][tempCol] > 0 && square[tempRow][tempCol] <squared){

if(oldRow == dimensions -1){
tempRow=0;
}

if(oldRow < dimensions -1){
tempRow = oldRow +1;
}

tempCol = oldCol;

if(tempRow >= dimensions && (tempCol >= 0 && tempCol < dimensions)){
tempRow = 0;
}
}

square[tempRow][tempCol] = increment;

oldRow = tempRow;
oldCol = tempCol;

increment++;

}

for(i=0; i< dimensions; i++){
for(j=0; j< dimensions; j++){
printf("%3d ",square[i][j]);
}
printf("\n");
}

printf("%d is the magic constant\n", constant);


/*This area is of the function is to decide whether or not the square is magic.
  Since I am not sure how to pass a 2D array to another function, I decided to
  keep it all within one function. Here, each row, column, and diagonal is checked
  and the sum of each one is compared to the magic constant. If the sum is equal to 
  the magic constant, nothing happens and at the end, if every sum passed this check
  a message stating that the square is indeed magic is printed to the screen. The
  first sum that is found, if any, that is not equal to the magic constant, a message
  is printed to thescreen stating why the square isnt magic and the program exits.*/
    
/*For loop to check each row*/
    
    for(i=0; i< dimensions; i++){
        sum=0;
        for(j=0; j< dimensions; j++){
            sum += square[i][j];
        }
        if(sum!=constant){
            printf("No: row %d is not equal to %d.\n",i, constant);
            exit(0);
        }
    }
    
/*For loop to check each column*/
    
    for(i=0; i< dimensions; i++){
        sum=0;
        for(j=0; j< dimensions; j++){
            sum += square[j][i];
        }
        if(sum!=constant){
            printf("No: column %d is not equal to %d.\n",i,constant);
            exit(0);
        }
    }
    
    sum=0;
    
/*For loop to check top-left to bottom-right diagonal*/

    for(i=0; i< dimensions; i++){
        sum += square[i][i];
    } 
    
    if(sum!=constant){
        printf("No: diagonal going from top-left to bottom-right is not equal to %d\n",constant);
        exit(0);
    }
    
    sum=0;
    j=dimensions-1;
    
    
/*For loop to check bottom-left to top-right diagonal*/
    
    for(i=0; i< dimensions; i++){
        sum += square[i][j];
        j--;
    } 
    
    if(sum!=constant){
        printf("No: diagonal going from bottom-left to top-right is not equal to %d\n",constant);
        exit(0);
    }
    
    printf("Yes this is a magic square!\n");
    
}
