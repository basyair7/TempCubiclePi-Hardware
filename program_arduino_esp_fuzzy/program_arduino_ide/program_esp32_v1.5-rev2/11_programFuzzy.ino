//***********************************************************************
// Support functions for Fuzzy Inference System                          
//***********************************************************************
// Triangular Member Function
FIS_TYPE fis_trimf(FIS_TYPE x, FIS_TYPE* p) {
  FIS_TYPE a = p[0], b = p[1], c = p[2];
  FIS_TYPE t1 = (x - a) / (b - a);
  FIS_TYPE t2 = (c - x) / (c - b);
  if ((a == b) && (b == c)) return (FIS_TYPE) (x == a);
  if (a == b) return (FIS_TYPE) (t2*(b <= x)*(x <= c));
  if (b == c) return (FIS_TYPE) (t1*(a <= x)*(x <= b));
  t1 = _min(t1, t2);
  return (FIS_TYPE) _max(t1, 0);
}

// Trapezoidal Member Function
FIS_TYPE fis_trapmf(FIS_TYPE x, FIS_TYPE* p) {
  FIS_TYPE a = p[0], b = p[1], c = p[2], d = p[3];
  FIS_TYPE t1 = ((x <= c) ? 1 : ((d < x) ? 0 : ((c != d) ? ((d - x) / (d - c)) : 0)));
  FIS_TYPE t2 = ((b <= x) ? 1 : ((x < a) ? 0 : ((a != b) ? ((x - a) / (b - a)) : 0)));
  return (FIS_TYPE) _min(t1, t2);
}

FIS_TYPE fis_prod(FIS_TYPE a, FIS_TYPE b) {
  return (a * b);
}

FIS_TYPE fis_probor(FIS_TYPE a, FIS_TYPE b) {
  return (a + b - (a * b));
}

FIS_TYPE fis_sum(FIS_TYPE a, FIS_TYPE b) {
  return (a + b);
}

FIS_TYPE fis_array_operation(FIS_TYPE *array, int size, _FIS_ARR_OP pfnOp) {
  int i;
  FIS_TYPE ret = 0;

  if (size == 0) return ret;
  if (size == 1) return array[0];

  ret = array[0];
  for (i = 1; i < size; i++) {
    ret = (*pfnOp)(ret, array[i]);
  }

  return ret;
}


//***********************************************************************
// Data for Fuzzy Inference System                                       
//***********************************************************************
// Pointers to the implementations of member functions
_FIS_MF fis_gMF[] = {
  fis_trimf, fis_trapmf
};

// Count of member function for each Input
int fis_gIMFCount[] = { 4, 4 };

// Count of member function for each Output 
int fis_gOMFCount[] = { 3, 3 };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFI0Coeff1[] = { 26, 30, 34 };
FIS_TYPE fis_gMFI0Coeff2[] = { 33, 37, 41 };
FIS_TYPE fis_gMFI0Coeff3[] = { 40, 45, 50.01 };
FIS_TYPE fis_gMFI0Coeff4[] = { 19.97, 23.5, 27 };
FIS_TYPE* fis_gMFI0Coeff[] = { fis_gMFI0Coeff1, fis_gMFI0Coeff2, fis_gMFI0Coeff3, fis_gMFI0Coeff4 };
FIS_TYPE fis_gMFI1Coeff1[] = { 30, 30, 38.35, 46 };
FIS_TYPE fis_gMFI1Coeff2[] = { 45, 52.5, 61 };
FIS_TYPE fis_gMFI1Coeff3[] = { 60, 67.5, 75 };
FIS_TYPE fis_gMFI1Coeff4[] = { 74, 81.65, 100, 100 };
FIS_TYPE* fis_gMFI1Coeff[] = { fis_gMFI1Coeff1, fis_gMFI1Coeff2, fis_gMFI1Coeff3, fis_gMFI1Coeff4 };
FIS_TYPE** fis_gMFICoeff[] = { fis_gMFI0Coeff, fis_gMFI1Coeff };

// Coefficients for the Output Member Functions
FIS_TYPE fis_gMFO0Coeff1[] = { 0, 0, 60 };
FIS_TYPE fis_gMFO0Coeff2[] = { 0, 0, 80 };
FIS_TYPE fis_gMFO0Coeff3[] = { 0, 0, 100 };
FIS_TYPE* fis_gMFO0Coeff[] = { fis_gMFO0Coeff1, fis_gMFO0Coeff2, fis_gMFO0Coeff3 };
FIS_TYPE fis_gMFO1Coeff1[] = { 0, 0, 0 };
FIS_TYPE fis_gMFO1Coeff2[] = { 0, 0, 65 };
FIS_TYPE fis_gMFO1Coeff3[] = { 0, 0, 80 };
FIS_TYPE* fis_gMFO1Coeff[] = { fis_gMFO1Coeff1, fis_gMFO1Coeff2, fis_gMFO1Coeff3 };
FIS_TYPE** fis_gMFOCoeff[] = { fis_gMFO0Coeff, fis_gMFO1Coeff };

// Input membership function set
int fis_gMFI0[] = { 0, 0, 0, 0 };
int fis_gMFI1[] = { 1, 0, 0, 1 };
int* fis_gMFI[] = { fis_gMFI0, fis_gMFI1};

// Output membership function set

int* fis_gMFO[] = {};

// Rule Weights
FIS_TYPE fis_gRWeight[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Type
int fis_gRType[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Inputs
int fis_gRI0[] = { 4, 4 };
int fis_gRI1[] = { 4, 3 };
int fis_gRI2[] = { 4, 2 };
int fis_gRI3[] = { 4, 1 };
int fis_gRI4[] = { 1, 4 };
int fis_gRI5[] = { 1, 3 };
int fis_gRI6[] = { 1, 2 };
int fis_gRI7[] = { 1, 1 };
int fis_gRI8[] = { 2, 4 };
int fis_gRI9[] = { 2, 3 };
int fis_gRI10[] = { 2, 2 };
int fis_gRI11[] = { 2, 1 };
int fis_gRI12[] = { 3, 4 };
int fis_gRI13[] = { 3, 3 };
int fis_gRI14[] = { 3, 2 };
int fis_gRI15[] = { 3, 1 };
int* fis_gRI[] = { fis_gRI0, fis_gRI1, fis_gRI2, fis_gRI3, fis_gRI4, fis_gRI5, fis_gRI6, fis_gRI7, fis_gRI8, fis_gRI9, fis_gRI10, fis_gRI11, fis_gRI12, fis_gRI13, fis_gRI14, fis_gRI15 };

// Rule Outputs
int fis_gRO0[] = { 3, 3 };
int fis_gRO1[] = { 3, 2 };
int fis_gRO2[] = { 2, 1 };
int fis_gRO3[] = { 1, 1 };
int fis_gRO4[] = { 3, 3 };
int fis_gRO5[] = { 3, 2 };
int fis_gRO6[] = { 2, 1 };
int fis_gRO7[] = { 1, 1 };
int fis_gRO8[] = { 3, 3 };
int fis_gRO9[] = { 3, 2 };
int fis_gRO10[] = { 2, 1 };
int fis_gRO11[] = { 1, 1 };
int fis_gRO12[] = { 2, 2 };
int fis_gRO13[] = { 2, 1 };
int fis_gRO14[] = { 1, 1 };
int fis_gRO15[] = { 1, 1 };
int* fis_gRO[] = { fis_gRO0, fis_gRO1, fis_gRO2, fis_gRO3, fis_gRO4, fis_gRO5, fis_gRO6, fis_gRO7, fis_gRO8, fis_gRO9, fis_gRO10, fis_gRO11, fis_gRO12, fis_gRO13, fis_gRO14, fis_gRO15 };

// Input range Min
FIS_TYPE fis_gIMin[] = { 20, 30 };

// Input range Max
FIS_TYPE fis_gIMax[] = { 50, 90 };

// Output range Min
FIS_TYPE fis_gOMin[] = { 40, 0 };

// Output range Max
FIS_TYPE fis_gOMax[] = { 100, 80 };

//***********************************************************************
// Data dependent support functions for Fuzzy Inference System           
//***********************************************************************
// None for Sugeno

//***********************************************************************
// Fuzzy Inference System                                                
//***********************************************************************
void fis_evaluate() {
  FIS_TYPE fuzzyInput0[] = { 0, 0, 0, 0 };
  FIS_TYPE fuzzyInput1[] = { 0, 0, 0, 0 };
  FIS_TYPE* fuzzyInput[fis_gcI] = { fuzzyInput0, fuzzyInput1, };
  FIS_TYPE fuzzyOutput0[] = { 0, 0, 0 };
  FIS_TYPE fuzzyOutput1[] = { 0, 0, 0 };
  FIS_TYPE* fuzzyOutput[fis_gcO] = { fuzzyOutput0, fuzzyOutput1, };
  FIS_TYPE fuzzyRules[fis_gcR] = { 0 };
  FIS_TYPE fuzzyFires[fis_gcR] = { 0 };
  FIS_TYPE* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
  FIS_TYPE sW = 0;

  // Transforming input to fuzzy Input
  int i, j, r, o;
  for (i = 0; i < fis_gcI; ++i) 
  {
    for (j = 0; j < fis_gIMFCount[i]; ++j) {
      fuzzyInput[i][j] = (fis_gMF[fis_gMFI[i][j]])(g_fisInput[i], fis_gMFICoeff[i][j]);
    }
  }

  int index = 0;
  for (r = 0; r < fis_gcR; ++r) {
    if (fis_gRType[r] == 1) {
      fuzzyFires[r] = 1;
      for (i = 0; i < fis_gcI; ++i) {
        index = fis_gRI[r][i];
        if (index > 0)
          fuzzyFires[r] = fis_prod(fuzzyFires[r], fuzzyInput[i][index - 1]);
        else if (index < 0)
          fuzzyFires[r] = fis_prod(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
        else
          fuzzyFires[r] = fis_prod(fuzzyFires[r], 1);
      }
    }
    else {
      fuzzyFires[r] = 0;
      for (i = 0; i < fis_gcI; ++i) {
        index = fis_gRI[r][i];
        if (index > 0)
            fuzzyFires[r] = fis_probor(fuzzyFires[r], fuzzyInput[i][index - 1]);
        else if (index < 0)
            fuzzyFires[r] = fis_probor(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
        else
            fuzzyFires[r] = fis_probor(fuzzyFires[r], 0);
      }
    }

    fuzzyFires[r] = fis_gRWeight[r] * fuzzyFires[r];
    sW += fuzzyFires[r];
  }

  if (sW == 0) {
    for (o = 0; o < fis_gcO; ++o) {
      g_fisOutput[o] = ((fis_gOMax[o] + fis_gOMin[o]) / 2);
    }
  }
  else {
    for (o = 0; o < fis_gcO; ++o) {
      FIS_TYPE sWI = 0.0;
      for (j = 0; j < fis_gOMFCount[o]; ++j) {
        fuzzyOutput[o][j] = fis_gMFOCoeff[o][j][fis_gcI];
        for (i = 0; i < fis_gcI; ++i)
        {
            fuzzyOutput[o][j] += g_fisInput[i] * fis_gMFOCoeff[o][j][i];
        }
      }

      for (r = 0; r < fis_gcR; ++r) {
        index = fis_gRO[r][o] - 1;
        sWI += fuzzyFires[r] * fuzzyOutput[o][index];
      }

      g_fisOutput[o] = sWI / sW;
    }
  }
}