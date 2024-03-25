#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int *pDataCopy, int dataSz)
{
  int tmp = 0;

  // # of leaf elements: (dataSz+1)/2,
  // index of last leaf element's parent = (dataSz/2)-1
  for (int i = (dataSz / 2) - 1; i >= 0; i--) {

      // Left child for current i
      int j = 2 * i + 1;  
    
      while(j < dataSz - 1) {
        
          // Compare left and right children of current i
          // If left child is greater than right child,
          // Increment j
          if (pDataCopy[j] > pDataCopy[j + 1]) {
              j = j + 1;
          }

          // Compare parent and largest child
          if (pDataCopy[i] > pDataCopy[j]) {
              tmp = pDataCopy[i];
              pDataCopy[i] = pDataCopy[j];
              pDataCopy[j] = tmp;
              i = j;
              j = 2 * i + 1;
          } 
            
          else {
              break;
          }
        
      }
  }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
  int m = 0;

    if (l < r)
    {
      // get the mid point
      m =  l + (r - l)/2;

      // Sort first and second halves
      // Also calling recursively to divide the original array
      mergeSort(pData, l, m);
      mergeSort(pData, m+1, r);

      //Initializing indexed for the first half, second half, and merged arrays.
      int i = 0, j = 0, k = 0;

      //Merging the subarrays into the sorted 
      //left and right halves of the original.
      int h1 = m - l + 1;
      int h2 = r - m;

      //Allocating temp arrays
      int *L = (int*)Alloc(h1 * sizeof(int));
      int *R = (int*)Alloc(h2 * sizeof(int));

      // Copy data to temp arrays L[] and R[] 
      for (i = 0; i < h1; i++)
        L[i] = pData[l + i];

      //Merge the temp arrays back into arr[l..r]
      for (j = 0; j < h2; j++)
        R[j] = pData[m + 1 + j];
      

      i = 0; // Initial index of first subarray
      j = 0; // Initial index of second subarray
      k = l; // Initial index of merged subarray

      
      while (i < h1 && j < h2)
      {
        if (L[i] <= R[j])
        {
          pData[k] = L[i];
          i++;
        }

        else	
        {
          pData[k] = R[j];
          j++;
        }
        
        k++;
      }

      // Copy the remaining elements of L[], if there are any
      while (i < h1)
      {
        pData[k] = L[i];
        i++;
        k++;
      }

      // Copy the remaining elements of R[], if there are any 
      while (j < h2)
      {
        pData[k] = R[j];
        j++;
        k++;	
      }

      //Deallocating temporary arrays.
      DeAlloc(L);
      DeAlloc(R);
    }
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
  int i = 0, j = 0, key = 0;
  
  for (i = 1; i < n; i++)
    {
      key = pData[i];
      
      //Move elements of arr[0..i-1], that are
      //greater than key, to one position ahead
      //of their current position 
      for(j = i-1; j >= 0; j--)
      {
        if(pData[j] > key)
        pData[j+1] = pData[j];
          
        else
        break;
      }

      //Place key at its correct position
      pData[j+1] = key;
  }
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
  int i = 0, j = 0, temp = 0;

    //Loop to access each array element
    for (i = 0; i < n-1; i++)
    {
      //Loop to compare array elements
      for (j = 0; j < n-i-1; j++)
      {
        //if the current element is greater than the next element
        if (pData[j] > pData[j+1])
          {//then swap
            temp = pData[j];
            pData[j] = pData[j+1];
            pData[j+1] = temp;
          }
      }
    }
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
  int i = 0, j = 0, min = 0, tmp = 0;

  // One by one move boundary of unsorted subarray
  for (i = 0; i < n-1; i++)
  {
    // Find the minimum element in unsorted array
    min = i;
    for (j = i+1; j < n; j++)
    if (pData[j] < pData[min])
    min = j;

    // Swap the found minimum element with the first element
    tmp = pData[i];
    pData[i] = pData[min];
    pData[min] = tmp;
  }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
  FILE* inFile = fopen(inputFileName,"r");
  int dataSz = 0;
  int i, n, *data;
  *ppData = NULL;

  if (inFile)
  {
    fscanf(inFile,"%d\n",&dataSz);
    *ppData = (int *)malloc(sizeof(int) * dataSz);
    // Implement parse data block
    if (*ppData == NULL)
    {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i=0;i<dataSz;++i)
    {
      fscanf(inFile, "%d ",&n);
      data = *ppData + i;
      *data = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
  double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
	  printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz); 
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz); 

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz); 
		
	  printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz); 

    printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz); 
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
