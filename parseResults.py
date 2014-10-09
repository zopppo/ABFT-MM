from __future__ import division
from decimal import *
import matplotlib.pyplot as plt
import os
file = open("result5.txt", "r")
rowErrors = 0
colErrors = 0
errorsCorrected = 0
recomputed = 0
recomputeCalled = 0
matricesCorrect = 0
run = 0


def parseInt(line, string):
    index = line.find(string) + len(string)
    num = line[index]
    index += 1
    while line[index].isdigit():
        num += line[index]
        index += 1
    return int(num)

def save(path, ext='png', close=True, verbose=True):
    """Save a figure from pyplot.
 
    Parameters
    ----------
    path : string
        The path (and filename, without the extension) to save the
        figure to.
 
    ext : string (default='png')
        The file extension. This must be supported by the active
        matplotlib backend (see matplotlib.backends module).  Most
        backends support 'png', 'pdf', 'ps', 'eps', and 'svg'.
 
    close : boolean (default=True)
        Whether to close the figure after saving.  If you want to save
        the figure multiple times (e.g., to multiple formats), you
        should NOT close it in between saves or you will have to
        re-plot it.
 
    verbose : boolean (default=True)
        Whether to print information about when and where the image
        has been saved.
 
    """
    
    # Extract the directory and filename from the given path
    directory = os.path.split(path)[0]
    filename = "%s.%s" % (os.path.split(path)[1], ext)
    if directory == '':
        directory = '.'
 
    # If the directory does not exist, create it
    if not os.path.exists(directory):
        os.makedirs(directory)
 
    # The final path to save to
    savepath = os.path.join(directory, filename)
 
    if verbose:
        print("Saving figure to '%s'..." % savepath),
 
    # Actually save the figure
    plt.savefig(savepath)
    
    # Close it
    if close:
        plt.close()
 
    if verbose:
        print("Done")

yAxis = []
getcontext().prec = 6

for line in file:
    if line.find('Run') != -1:
        if line == 'Run 1':
            continue
        rowErrors /= 10
        colErrors /= 10
        yAxis.append(str(Decimal(rowErrors) * Decimal(colErrors)))
        errorsCorrected /= 10
        recomputed /= 10
        matricesCorrect /= 10
##        print("Run", run)
##        print("rowErrors: ", rowErrors)
##        print("colErrors: ", colErrors)
##        print("errorsCorrected: ", errorsCorrected)
##        print("matricesCorrect: ", matricesCorrect)
##        print("recomputed: ", recomputed)
        run += 1
        rowErrors = 0
        colErrors = 0
        errorsCorrected = 0
        recomputed = 0
        recomputeCalled = 0
        matricesCorrect = 0
    else:
        rowErrors += parseInt(line, "rE: ")
        colErrors += parseInt(line, "cE: ")
        errorsCorrected += parseInt(line, "Corrected: ")
        recomputed += parseInt(line, "Recomputed: ")
        recomputeCalled += parseInt(line, "Recompute called: ")
        if line.find("Not Correct") == -1:
            matricesCorrect += 1

xAxis = []
getcontext().prec = 2
prob = .001
for i in range(0, 100):
    xAxis.append(str(round(prob, 3)))
    prob += .001
plt.xlabel('Probability %')
plt.ylabel('Errors')
plt.plot(xAxis, yAxis)
save("graph", ext="png", close=False, verbose=True)
