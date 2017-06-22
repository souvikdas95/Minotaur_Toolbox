#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "BOOL.h"
#include <localization.h>

#define isDoubleIntegerType(x, y)	((double)((y)x)==x)

int getInt32FromScilab(int argNum, int* dest)
{
	SciErr sciErr;
	int iRet = 0;
	int *varAddress = NULL;
	double inputDouble = 0;
	const char errMsg[] = "Wrong type for input argument #%d: An Integer is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isDoubleType(pvApiCtx, varAddress) || isVarComplex(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum);
		return 1;
	}
	iRet = getScalarDouble(pvApiCtx, varAddress, &inputDouble);
	if(iRet || !isDoubleIntegerType(inputDouble, int))
	{
		Scierror(errNum, errMsg, argNum);
		return 1;
	}
	*dest = (int)inputDouble;
	return 0;
}

int getInt32MatrixFromScilab(int argNum, int *rows, int *cols, int **dest)
{
	SciErr sciErr;
	int* varAddress = NULL;
	double* destDouble = NULL;
	const char errMsg[] = "Wrong type for input argument #%d: A matrix of Integer is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isDoubleType(pvApiCtx, varAddress) || isVarComplex(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum);
		return 1;
	}
	sciErr = getMatrixOfDouble(pvApiCtx, varAddress, rows, cols, &destDouble);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	int i;
	*dest = (int*)malloc(sizeof(int) * (*rows) * (*cols));
	for(i = 0; i < (*rows) * (*cols); ++i)
	{
		if(!isDoubleIntegerType(destDouble[i], int))
		{
			Scierror(errNum, errMsg, argNum);
			return 1;
		}
		(*dest)[i] = (int)destDouble[i];
	}
	return 0;
}

int getFixedSizeInt32MatrixFromScilab(int argNum, int rows, int cols, int **dest)
{
	SciErr sciErr;
	int* varAddress = NULL;
	int inputMatrixRows = 0, inputMatrixCols = 0;
	double* destDouble = NULL;
	const char errMsg[] = "Wrong type for input argument #%d: A matrix of Integer of size %d by %d is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isDoubleType(pvApiCtx, varAddress) || isVarComplex(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum, rows, cols);
		return 1;
	}
	sciErr = getMatrixOfDouble(pvApiCtx, varAddress, &inputMatrixRows, &inputMatrixCols, &destDouble);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if(inputMatrixRows != rows || inputMatrixCols != cols)
	{
		Scierror(errNum, errMsg, argNum, rows, cols);
		return 1;
	}
	int i;
	*dest = (int*)malloc(sizeof(int) * rows * cols);
	for(i = 0; i < rows * cols; ++i)
	{
		if(!isDoubleIntegerType(destDouble[i], int))
		{
			Scierror(errNum, errMsg, argNum, rows, cols);
			return 1;
		}
		(*dest)[i] = (int)destDouble[i];
	}
	return 0;
}

int returnInt32ToScilab(int itemPos, int retVal)
{
	int iRet;
	iRet = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+itemPos, (double)retVal);
	if(iRet)
	{
		AssignOutputVariable(pvApiCtx, itemPos) = 0;
		return 1;
	}
	AssignOutputVariable(pvApiCtx, itemPos) = nbInputArgument(pvApiCtx)+itemPos;
	return 0;
}

int returnInt32MatrixToScilab(int itemPos, int rows, int cols, int *dest)
{
	SciErr sciErr;
	double* destDouble = NULL;
	int i;
	destDouble = (double*)malloc(sizeof(double) * rows * cols);
	for(i = 0; i < rows * cols; ++i)
	{
		destDouble[i] = (double)dest[i];
	}
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + itemPos, rows, cols, destDouble);
	free(destDouble);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	AssignOutputVariable(pvApiCtx, itemPos) = nbInputArgument(pvApiCtx)+itemPos;
	return 0;
}

int getInt64FromScilab(int argNum, long long* dest)
{
	SciErr sciErr;
	int iRet = 0;
	int *varAddress = NULL;
	double inputDouble = 0;
	const char errMsg[] = "Wrong type for input argument #%d: An Integer is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isDoubleType(pvApiCtx, varAddress) || isVarComplex(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum);
		return 1;
	}
	iRet = getScalarDouble(pvApiCtx, varAddress, &inputDouble);
	if(iRet || !isDoubleIntegerType(inputDouble, long long))
	{
		Scierror(errNum, errMsg, argNum);
		return 1;
	}
	*dest = (long long)inputDouble;
	return 0;
}

int getInt64MatrixFromScilab(int argNum, int *rows, int *cols, long long **dest)
{
	SciErr sciErr;
	int* varAddress = NULL;
	double* destDouble = NULL;
	const char errMsg[] = "Wrong type for input argument #%d: A matrix of Integer is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isDoubleType(pvApiCtx, varAddress) || isVarComplex(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum);
		return 1;
	}
	sciErr = getMatrixOfDouble(pvApiCtx, varAddress, rows, cols, &destDouble);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	int i;
	*dest = (long long*)malloc(sizeof(long long) * (*rows) * (*cols));
	for(i = 0; i < (*rows) * (*cols); ++i)
	{
		if(!isDoubleIntegerType(destDouble[i], long long))
		{
			Scierror(errNum, errMsg, argNum);
			return 1;
		}
		(*dest)[i] = (long long)destDouble[i];
	}
	return 0;
}

int getFixedSizeInt64MatrixFromScilab(int argNum, int rows, int cols, long long** dest)
{
	SciErr sciErr;
	int* varAddress = NULL;
	int inputMatrixRows = 0, inputMatrixCols = 0;
	double* destDouble = NULL;
	const char errMsg[] = "Wrong type for input argument #%d: A matrix of Integer of size %d by %d is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isDoubleType(pvApiCtx, varAddress) || isVarComplex(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum, rows, cols);
		return 1;
	}
	sciErr = getMatrixOfDouble(pvApiCtx, varAddress, &inputMatrixRows, &inputMatrixCols, &destDouble);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if(inputMatrixRows != rows || inputMatrixCols != cols)
	{
		Scierror(errNum, errMsg, argNum, rows, cols);
		return 1;
	}
	int i;
	*dest = (long long*)malloc(sizeof(long long) * rows * cols);
	for(i = 0; i < rows * cols; ++i)
	{
		if(!isDoubleIntegerType(destDouble[i], long long))
		{
			Scierror(errNum, errMsg, argNum, rows, cols);
			return 1;
		}
		(*dest)[i] = (long long)destDouble[i];
	}
	return 0;
}

int returnInt64ToScilab(int itemPos, long long retVal)
{
	int iRet;
	iRet = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+itemPos, (double)retVal);
	if(iRet)
	{
		AssignOutputVariable(pvApiCtx, itemPos) = 0;
		return 1;
	}
	AssignOutputVariable(pvApiCtx, itemPos) = nbInputArgument(pvApiCtx)+itemPos;
	return 0;
}

int returnInt64MatrixToScilab(int itemPos, int rows, int cols, long long *dest)
{
	SciErr sciErr;
	double* destDouble = NULL;
	int i;
	destDouble = (double*)malloc(sizeof(double) * rows * cols);
	for(i = 0; i < rows * cols; ++i)
	{
		destDouble[i] = (double)dest[i];
	}
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + itemPos, rows, cols, destDouble);
	free(destDouble);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	AssignOutputVariable(pvApiCtx, itemPos) = nbInputArgument(pvApiCtx)+itemPos;
	return 0;
}

int returnBooleanMatrixToScilab(int itemPos, int rows, int cols, BOOL* dest)
{	
	SciErr sciErr;
	sciErr = createMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + itemPos, rows, cols, (const int*)dest);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	AssignOutputVariable(pvApiCtx, itemPos) = nbInputArgument(pvApiCtx)+itemPos;
	return 0;
}

int getDoubleFromScilab(int argNum, double* dest)
{
	SciErr sciErr;
	int iRet = 0;
	int *varAddress = NULL;
	const char errMsg[] = "Wrong type for input argument #%d: An Integer is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isDoubleType(pvApiCtx, varAddress) || isVarComplex(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum);
		return 1;
	}
	iRet = getScalarDouble(pvApiCtx, varAddress, dest);
	return 0;
}

int getFixedSizeDoubleMatrixFromScilab(int argNum, int rows, int cols, double** dest)
{
	SciErr sciErr;
	int* varAddress = NULL;
	int inputMatrixRows = 0, inputMatrixCols = 0;
	const char errMsg[] = "Wrong type for input argument #%d: A matrix of Double of size %d by %d is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isDoubleType(pvApiCtx, varAddress) || isVarComplex(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum, rows, cols);
		return 1;
	}
	sciErr = getMatrixOfDouble(pvApiCtx, varAddress, &inputMatrixRows, &inputMatrixCols, dest);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if(inputMatrixRows != rows || inputMatrixCols != cols)
	{
		Scierror(errNum, errMsg, argNum, rows, cols);
		return 1;
	}
	return 0;
}

int getFixedSizeListOfFixedSizeDoubleMatrixFromScilab(int argNum, int rows, int cols, double*** dest, int len)
{
	SciErr sciErr;
	int i;
	int* varAddress = NULL;
	int* varItemAddress = NULL;
	int inputMatrixRows = 0, inputMatrixCols = 0, inputListLen = 0;
	const char errMsg[] = "Wrong type for input argument #%d: A List of length %d containing matrix of Double of size %d by %d is expected.\n";
	const int errNum = 999;
	sciErr = getVarAddressFromPosition(pvApiCtx, argNum, &varAddress);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (!isListType(pvApiCtx, varAddress))
	{
		Scierror(errNum, errMsg, argNum, len, rows, cols);
		return 1;
	}
	sciErr = getListItemNumber(pvApiCtx, varAddress, &inputListLen);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	if (inputListLen != len)
	{
		Scierror(errNum, errMsg, argNum, len, rows, cols);
		return 1;
	}
	*dest = (double**)malloc(sizeof(double**) * len);
	for(i = 1; i <= len; ++i)
	{
		sciErr = getListItemAddress(pvApiCtx, varAddress, i, &varItemAddress);
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 1;
		}
		if (!isDoubleType(pvApiCtx, varItemAddress) || isVarComplex(pvApiCtx, varItemAddress))
		{
			Scierror(errNum, errMsg, argNum, len, rows, cols);
			return 1;
		}
		sciErr = getMatrixOfDouble(pvApiCtx, varItemAddress, &inputMatrixRows, &inputMatrixCols, &((*dest)[i - 1]));
		if (sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 1;
		}
		if(inputMatrixRows != rows || inputMatrixCols != cols)
		{
			Scierror(errNum, errMsg, argNum, len, rows, cols);
			return 1;
		}
	}
	return 0;
}

int returnDoubleToScilab(int itemPos, double retVal)
{
	int iRet;
	iRet = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+itemPos,retVal);
	if(iRet)
	{
		AssignOutputVariable(pvApiCtx, itemPos) = 0;
		return 1;
	}
	AssignOutputVariable(pvApiCtx, itemPos) = nbInputArgument(pvApiCtx)+itemPos;
	return 0;
}

int returnDoubleMatrixToScilab(int itemPos, int rows, int cols, double *dest)
{
	SciErr sciErr;
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + itemPos, rows, cols, dest);
	if (sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	AssignOutputVariable(pvApiCtx, itemPos) = nbInputArgument(pvApiCtx)+itemPos;
	return 0;
}