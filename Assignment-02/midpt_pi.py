import sys

N_pts = int(sys.argv[1]) # Read number of rectangles from command-line
delta_x = 1/float(N_pts) # Width of subintervals/rectangles

# Main loop
pi_est = 0.0
for k in range(N_pts):
    x_k = (k+0.5)*delta_x
    pi_est += 1.0/(1.0+x_k**2)

pi_est *= (4.0*delta_x)
print "%17.15f" % pi_est
