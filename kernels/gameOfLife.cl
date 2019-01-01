__kernel void gameOfLife(__global short* currentMatrix, __global short* nextMatrix, const int cols, const int rows) {

    int id, row, col;
    id = get_global_id(0);
    col = id % cols;
    row = id / rows;

    int center, NO, N, NE, O, E, SO, S, SE;


    // Debe ser circular
    center = row*rows + col;
    NO = ((rows + row - 1)%rows)*rows + ((cols + col - 1)%cols);
    N = ((rows + row - 1)%rows)*rows + col;
    NE = ((rows + row - 1)%rows)*rows + ((cols + col + 1)%cols);
    O = row*rows + ((cols + col - 1)%cols);
    E = row*rows + ((cols + col + 1)%cols);
    SO = ((rows + row + 1)%rows)*rows + ((cols + col - 1)%cols);
    S = ((rows + row + 1)%rows)*rows + col;
    SE = ((rows + row + 1)%rows)*rows + ((cols + col + 1)%cols);

    // printf("Kernel %d NO %d N %d NE %d O %d center %d E %d SO %d S %d SE %d", id, currentMatrix[NO], currentMatrix[N], currentMatrix[NE], currentMatrix[O], currentMatrix[center], currentMatrix[E], currentMatrix[SO], currentMatrix[S], currentMatrix[SE]);
    // printf("Kernel %d NO %d N %d NE %d O %d center %d E %d SO %d S %d SE %d", id, NO, N, NE, O, center, E, SO, S, SE);

    int aliveNeighbors = currentMatrix[NO] + currentMatrix[N] + currentMatrix[NE] +
                         currentMatrix[O] + currentMatrix[E] +
                         currentMatrix[SO] + currentMatrix[S] + currentMatrix[SE];

    // Comprobar la vecindad
    // asignar valor a next
    if (aliveNeighbors == 3 || (aliveNeighbors == 2 && currentMatrix[center]==1)) {
      nextMatrix[center] = 1;
    }
    else {
      nextMatrix[center] = 0;
    }
}