import math
TOL = 1.0e-16
def exp_taylor(x, N_terms):
    term = 1.0
    partial_sum = term
    for j in range(1,N_terms+1): # Terms computed rcursively
        term *= x/float(j)       # jth term = x**j / (j!)
        partial_sum += term      # Partial sum incremented in-place
	if (abs(term) < TOL * abs(partial_sum)):
	    break                # Halt when terms sufficiently small
    return (partial_sum,j)       # returns value & number or terms summed

def test_exp_taylor():
    N_max = 100
    R = 20  
    step = 4
    header = "x".center(10) + "# terms".rjust(8) + "true".rjust(19) 
    header += "approximate".rjust(19) + "error".rjust(19) 
    print header
    print "="*(10+19+19+19+8)
    for ell in range(-R,R+1,step):
        x = float(ell)
	(y,N_terms) = exp_taylor(x, N_max)
        exp_true = math.exp(x)
	relative_error = abs( (y-exp_true) / exp_true )
        print "%10.5f%8d%19g%19g%19g" % (x,N_terms,exp_true,y,relative_error)

if __name__=='__main__':
    test_exp_taylor()
