import pandas as pd
import sys
import matplotlib.pyplot as plt
import warnings
import seaborn as sns

if __name__ == '__main__':
    penguins = pd.read_csv(sys.argv[1])

    print("###### Atribut exploration\n")

    print("#### Kategorické\n")
    include = ['object']
    print(penguins.describe(include = include))
    print("")


    print("## Species\n--------")
    print(penguins.species.value_counts())
    print(penguins.species.value_counts(normalize=True).mul(100).round(1).astype(str)+'%')
    print("")

    print("## Island\n--------")
    print(penguins.island.value_counts())
    print(penguins.island.value_counts(normalize=True).mul(100).round(1).astype(str)+'%')
    print("")

    print("## Sex\n--------")
    print(penguins.sex.value_counts())
    print(penguins.sex.value_counts(normalize=True).mul(100).round(1).astype(str)+'%')
    print("")


    print("#### Číselné\n")
    print(penguins.describe())
    print("")

    print("###### Missing values\n")
    print(penguins.isnull().sum())
    print("")
    sum = 0
    moreThanTwo = 0
    for i in penguins.index:
        amount = penguins.loc[i].isnull().sum()
        if amount > 0:
            sum = sum + 1
            print(f'Record with missing value {sum} on row {i}')
            print(penguins.loc[i])
            print("")
            if amount > 1:
                moreThanTwo = moreThanTwo + 1
    print(f'Total rows with missing value: {sum}')
    print(f'Total rows with more than two missing value: {moreThanTwo}')
    print("")

    ###### GRAPHS ########
    warnings.filterwarnings(action='ignore', category=UserWarning)

    male = penguins.loc[ penguins['sex']=='MALE'].dropna()
    female = penguins.loc[ penguins['sex']=='FEMALE'].dropna()

    adelie = penguins.loc[ penguins['species']=='Adelie'].dropna()
    chinstrap = penguins.loc[ penguins['species']=='Chinstrap'].dropna()
    gentoo = penguins.loc[ penguins['species']=='Gentoo'].dropna()
    speciesColors = ['#eeee00', '#00ee00', '#00eeee']
    sexColor = ['#ee0000', '#0000ee']

    dataSexLength = [female['culmen_length_mm'], male['culmen_length_mm']]
    dataSexDepth = [female['culmen_depth_mm'], male['culmen_depth_mm']]
    dataSexFlipper = [female['flipper_length_mm'], male['flipper_length_mm']]
    dataSexMass = [female['body_mass_g'], male['body_mass_g']]
    
    dataSpeciesLength = [adelie['culmen_length_mm'], chinstrap['culmen_length_mm'], gentoo['culmen_length_mm']]
    dataSpeciesDepth = [adelie['culmen_depth_mm'], chinstrap['culmen_depth_mm'], gentoo['culmen_depth_mm']]
    dataSpeciesFlipper = [adelie['flipper_length_mm'], chinstrap['flipper_length_mm'], gentoo['flipper_length_mm']]
    dataSpeciesMass = [adelie['body_mass_g'], chinstrap['body_mass_g'], gentoo['body_mass_g']]

    torgersen = penguins.loc[ penguins['island']=='Torgersen'].dropna()
    biscoe = penguins.loc[ penguins['island']=='Biscoe'].dropna()
    dream = penguins.loc[ penguins['island']=='Dream'].dropna()

    columnName = ['culmen length (mm)', 'culmen depth (mm)', 'flipper length (mm)', 'body mass (g)']
    rowName = ['sex', 'species']
    speciesName = ['Adelie', 'Chinstrap', 'Gentoo']
    sexName = ['Female', 'Male']

    torgersen.species.hist()
    plt.title("Torgersen")
    plt.show()
    biscoe.species.hist()
    plt.title("Biscoe")
    plt.show()

    dream.species.hist()
    plt.title("Dream")
    plt.show()

    fig, axs = plt.subplots(2,4)

    for ax, col in zip(axs[0], columnName):
        ax.set_title(col)
    
    for ax, row in zip(axs[:,0], rowName):
        ax.set_ylabel(row, rotation=90, size='large')

    axs[0,0].set_xticklabels(sexName)
    box = axs[0,0].boxplot(dataSexLength, patch_artist=True)
    for patch, color in zip(box['boxes'], sexColor):
        patch.set_facecolor(color)

    axs[0,1].set_xticklabels(sexName)
    box = axs[0,1].boxplot(dataSexLength, patch_artist=True)
    for patch, color in zip(box['boxes'], sexColor):
        patch.set_facecolor(color)

    axs[0,2].set_xticklabels(sexName)
    box = axs[0,2].boxplot(dataSexLength, patch_artist=True)
    for patch, color in zip(box['boxes'], sexColor):
        patch.set_facecolor(color)

    axs[0,3].set_xticklabels(sexName)
    box = axs[0,3].boxplot(dataSexLength, patch_artist=True)
    for patch, color in zip(box['boxes'], sexColor):
        patch.set_facecolor(color)

    axs[1,0].set_xticklabels(speciesName)
    box = axs[1,0].boxplot(dataSpeciesLength, patch_artist=True)
    for patch, color in zip(box['boxes'], speciesColors):
        patch.set_facecolor(color)

    axs[1,1].set_xticklabels(speciesName)
    box = axs[1,1].boxplot(dataSpeciesDepth, patch_artist=True)
    for patch, color in zip(box['boxes'], speciesColors):
        patch.set_facecolor(color)

    axs[1,2].set_xticklabels(speciesName)
    box = axs[1,2].boxplot(dataSpeciesFlipper, patch_artist=True)
    for patch, color in zip(box['boxes'], speciesColors):
        patch.set_facecolor(color)

    axs[1,3].set_xticklabels(speciesName)
    box = axs[1,3].boxplot(dataSpeciesMass, patch_artist=True)
    for patch, color in zip(box['boxes'], speciesColors):
        patch.set_facecolor(color)

    sns.pairplot(male,hue='species')
    sns.pairplot(female,hue='species')

    cm = male[['culmen_length_mm','culmen_depth_mm','flipper_length_mm','body_mass_g']].corr()
    cf = female[['culmen_length_mm','culmen_depth_mm','flipper_length_mm','body_mass_g']].corr()

    ca = adelie[['culmen_length_mm','culmen_depth_mm','flipper_length_mm','body_mass_g']].corr()
    cc = chinstrap[['culmen_length_mm','culmen_depth_mm','flipper_length_mm','body_mass_g']].corr()
    cg = gentoo[['culmen_length_mm','culmen_depth_mm','flipper_length_mm','body_mass_g']].corr()

    ct = torgersen[['culmen_length_mm','culmen_depth_mm','flipper_length_mm','body_mass_g']].corr()
    cb = biscoe[['culmen_length_mm','culmen_depth_mm','flipper_length_mm','body_mass_g']].corr()
    cd = dream[['culmen_length_mm','culmen_depth_mm','flipper_length_mm','body_mass_g']].corr()

    print("### Male correlation")
    print(cm)
    print("")
    print("### Female correlation")
    print(cf)
    print("")

    print("### Adelie correlation")
    print(ca)
    print("")
    print("### Chinstrap correlation")
    print(cc)
    print("")
    print("### Gentoo correlation")
    print(cg)
    print("")

    print("### Torgersen correlation")
    print(ct)
    print("")
    print("### Biscoe correlation")
    print(cb)
    print("")
    print("### Dream correlation")
    print(cd)
    print("")


    plt.figure(figsize=(10, 8))
    heatmap = sns.heatmap(cm, vmin=-1, vmax=1, annot=True)
    heatmap.set_title('Correlation male', fontdict={'fontsize':12}, pad=12)

    plt.figure(figsize=(10, 8))
    heatmap = sns.heatmap(cf, vmin=-1, vmax=1, annot=True)
    heatmap.set_title('Correlation female', fontdict={'fontsize':12}, pad=12)
    
    plt.figure(figsize=(10, 8))
    heatmap = sns.heatmap(ca, vmin=-1, vmax=1, annot=True)
    heatmap.set_title('Correlation Adelie', fontdict={'fontsize':12}, pad=12)
    
    plt.figure(figsize=(10, 8))
    heatmap = sns.heatmap(cc, vmin=-1, vmax=1, annot=True)
    heatmap.set_title('Correlation Chinstrap', fontdict={'fontsize':12}, pad=12)
    
    plt.figure(figsize=(10, 8))
    heatmap = sns.heatmap(cg, vmin=-1, vmax=1, annot=True)
    heatmap.set_title('Correlation Gentoo', fontdict={'fontsize':12}, pad=12)
    
    plt.figure(figsize=(10, 8))
    heatmap = sns.heatmap(ct, vmin=-1, vmax=1, annot=True)
    heatmap.set_title('Correlation Torgersen', fontdict={'fontsize':12}, pad=12)
    
    plt.figure(figsize=(10, 8))
    heatmap = sns.heatmap(cb, vmin=-1, vmax=1, annot=True)
    heatmap.set_title('Correlation Biscoe', fontdict={'fontsize':12}, pad=12)
    
    plt.figure(figsize=(10, 8))
    heatmap = sns.heatmap(cd, vmin=-1, vmax=1, annot=True)
    heatmap.set_title('Correlation Dream', fontdict={'fontsize':12}, pad=12)

    plt.show()
