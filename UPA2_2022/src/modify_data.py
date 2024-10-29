#3.py - Uprava datove sady do podoby vhodne pro dolovani

import numpy as np
import pandas as pd
import sys

# Creates and array specifiing ranges of bins
# Bins have equal width
def create_bins(min, max, N):
    assert(min < max and type(N) is int and N > 0)
    ret = [min + i*((max-min)/N) for i in range(N+1)]
    ret[0] -= 0.000001
    return ret

# Deletes columns specified in columns_list
def delete_columns(DF, columns_list):
    for column_name in columns_list:
        DF.pop(column_name)

# Deals with missing attributes for task A
def missing_attribs(DF):
    # Deleting rows where important attributes are missing
    rows_to_del = []
    for row in DF.index:
        if DF["Flipper Length (mm)"][row] is np.nan or DF["Sex"][row] is np.nan or DF["Sex"][row] == ".":
            rows_to_del += [row]
    DF.drop(rows_to_del, inplace=True)

    # Filling missing attributes with median calculated in step 2  
    DF['Delta 13 C (o/oo)'] = DF['Delta 13 C (o/oo)'].replace([np.nan], -25.83352)
    DF['Delta 15 N (o/oo)'] = DF['Delta 15 N (o/oo)'].replace([np.nan], 8.652405)

# Replaces numerical data with category data
def binned_column(DF, column_name, binsN=20):
    bins = create_bins(DF[column_name].min(), DF[column_name].max(), binsN)
    DF[column_name] = pd.cut(DF[column_name], bins)

# Converts categorical values to numerical values
# Normalizes some columns
def num_nor(DF):
    # Make numerical values
    DF['Sex'] = DF['Sex'].replace("MALE", 0.0)
    DF['Sex'] = DF['Sex'].replace("FEMALE", 1.0)
    DF['Species'] = DF['Species'].replace("Adelie Penguin (Pygoscelis adeliae)", 1.0)
    DF['Species'] = DF['Species'].replace("Chinstrap penguin (Pygoscelis antarctica)", 2.0)
    DF['Species'] = DF['Species'].replace("Gentoo penguin (Pygoscelis papua)", 3.0)

    # Normalize numerical values
    DF["Delta 13 C (o/oo)"] = (DF["Delta 13 C (o/oo)"] - DF["Delta 13 C (o/oo)"].min()) / (DF["Delta 13 C (o/oo)"].max() - DF["Delta 13 C (o/oo)"].min())
    DF["Delta 15 N (o/oo)"] = (DF["Delta 15 N (o/oo)"] - DF["Delta 15 N (o/oo)"].min()) / (DF["Delta 15 N (o/oo)"].max() - DF["Delta 15 N (o/oo)"].min())

    # Rename columns
    DF.rename(columns = {"Delta 13 C (o/oo)":"Delta 13 C (norm.)", "Delta 15 N (o/oo)":"Delta 15 N (norm.)", "Sex":"Sex (num)", "Species": "Species (num)"}, inplace = True)

if __name__ == "__main__":
    assert(len(sys.argv) == 4)
    print("------------ UPRAVA DATOVE SADY ------------")
    print(f"Vstupni csv:    {sys.argv[1]}")
    print(f"Vystupni csv A: {sys.argv[2]}")
    print(f"Vystupni csv B: {sys.argv[3]}")
    print("--------------------------------------------")
    # penguins A - datova sada pro kategoricka data
    # penguins B - datova sada pro numericka date
    penguins_A = pd.read_csv(sys.argv[1])
    penguins_B = pd.read_csv(sys.argv[1])

    print("(1)  - Odstraneni zbytecnych sloupcu")
    delete_columns(penguins_A, ["studyName", "Sample Number", "Region", "Stage", "Individual ID", "Clutch Completion", "Date Egg", "Comments"])
    delete_columns(penguins_B, ["studyName", "Sample Number", "Region", "Island", "Stage", "Individual ID", "Clutch Completion", "Date Egg", "Comments"])

    print("(2)  - Vyporadani se s chybejicimi atributy")
    missing_attribs(penguins_A)
    missing_attribs(penguins_B)

    print("(3A) - prevody do kosu")
    binned_column(penguins_A, 'Delta 13 C (o/oo)')
    binned_column(penguins_A, 'Delta 15 N (o/oo)')
    binned_column(penguins_A, 'Body Mass (g)')
    binned_column(penguins_A, 'Flipper Length (mm)')
    binned_column(penguins_A, 'Culmen Depth (mm)')
    binned_column(penguins_A, 'Culmen Length (mm)')

    print("(3B) - Numerizace a normalizace")
    num_nor(penguins_B)

    print("(4)  - Ulozeni vystupu")
    penguins_A.to_csv(sys.argv[2], index=False)
    penguins_B.to_csv(sys.argv[3], index=False)
    print("--------------------------------------------")