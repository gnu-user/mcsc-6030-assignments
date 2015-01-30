import numpy as np
TOL = 1.0e-16


def exp_taylor(x, N_terms):
    partial_sums = np.zeros_like(x)
    terms_summed = np.zeros_like(x)

    # An array of the N_terms
    N_array = np.arange(1, N_terms+1, dtype=np.float64)
    N_array = 1 / N_array

    # Calculate the terms array as outer product
    terms = np.outer(x, N_array)
    terms = np.where(terms < 0, -1*terms, terms)

    # Use a loop to accumulate the partial sums
    for i in range(terms.shape[0]):
        partial_sum = 0.0
        for j in range(1, terms.shape[1]):
            terms[i,j] *= terms[i, j-1]  # jth term = x**j / (j!)
            partial_sums[i] += terms[i,j]  # Partial sum incremented in-place
            terms_summed[i] = j
            # I do not know of any easy way to do this using "where"
            if (abs(terms[i,j]) < TOL * abs(partial_sums[i])):
                break
    # Get the inverse partial sum for any negative x values
    partial_sums = np.where(partial_sums < 0, 1 / partial_sums, partial_sums)

    return (partial_sums, terms_summed)  # returns value & number or terms summed


def test_exp_taylor():
    N_max = 100
    R = 20
    step = 4

    header = "x".center(10) + "# terms".rjust(8) + "true".rjust(19) 
    header += "approximate".rjust(19) + "error".rjust(19) 
    print header
    print "="*(10+19+19+19+8)

    # Generate the numpy array of x values
    x = np.arange(-R, R+1, step)

    # Perform only one call to exp_taylor function
    (y, N_terms) = exp_taylor(x, N_max)
    exp_true = np.exp(x)

    # Display the results
    for i in range(y.shape[0]):
        relative_error = abs((y[i]-exp_true[i]) / exp_true[i])
        print "%10.5f%8d%19g%19g%19g" % (x[i], N_terms[i], exp_true[i],
                                         y[i], relative_error)

if __name__=='__main__':
    test_exp_taylor()
