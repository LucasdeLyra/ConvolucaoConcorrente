import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
pd.set_option('display.max_rows', 5000)
COLUMNS = ['read', 'pre_processing', 'create_kernel', 'convolution', 'write']
images = {'minuscula': '236x236', 'pequena': '615x407', 'media': '2480x1365', 'grande': '5600x3200'}


def merge_all_csv_from_directory(directory):
    final_df = 0

    final_df = pd.read_csv(rf'{directory}/sequencial.txt', sep=';', header=None, names=COLUMNS)
    
    final_df['tempo_total'] = final_df.sum(axis=1)
    final_df = final_df.reset_index()
    num_threads = [0]*len(final_df)
    final_df['Threads'] = num_threads
    
    i = 1
    while i <= 8:
        df = pd.read_csv(rf'{directory}/{i}_threads.txt', sep=';', header=None, names=COLUMNS)
        df['tempo_total'] = df.sum(axis=1)
        df = df.reset_index()
        num_threads = [i]*len(df)
        df['Threads'] = num_threads
        final_df= final_df.append(df, ignore_index=True)
        i *= 2
    
    return final_df


def get_altogether_convolution_plot(column_name, title):
    for kernel in ['identidade', 'media']:
        for image_size in images:
            dir = rf'./../tests/eficiencia/{kernel}/imagem_{image_size}'
            merged_csv = merge_all_csv_from_directory(dir)

            fig = sns.relplot(data=merged_csv, x='index', y=f'{column_name}', hue='Threads', hue_order=[0,1,2,4,8], aspect=1.7, palette='deep')
            fig.set(xlabel = 'Iterações', ylabel = 'Tempo (s)', title = f'{title} para uma imagem de {images[image_size]}')
            fig.savefig(rf'./plots/{kernel}/{kernel}_{column_name}_{image_size}.png')
            plt.close()
            
            """fig = sns.relplot(data=merged_csv, x='index', y=f'{column_name}', col='Threads', aspect=0.5)
            fig.set(xlabel = 'Iterações', ylabel = 'Tempo (s)')
            fig.savefig(rf'./plots/{column_name}_{kernel}_{image_size}_separado.png')
            plt.close()"""


get_altogether_convolution_plot('convolution', 'Tempo de convolução')
get_altogether_convolution_plot('read', 'Tempo de leitura')
get_altogether_convolution_plot('pre_processing', 'Tempo de pré-processamento')
get_altogether_convolution_plot('write', 'Tempo de escrita')
get_altogether_convolution_plot('tempo_total', 'Tempo total')
















"""df = merge_all_csv_from_directory(r'./../tests/eficiencia/identidade/imagem_media')
df2 = pd.DataFrame({'Media': df.groupby('Threads')['convolution'].mean()})
df = df.merge(df2, on='Threads', how='left')

fig  = sns.scatterplot(data=df, x='index', y='convolution', hue='Threads', hue_order=[0,1,2,4,8], legend='auto', palette='deep')
sns.lineplot(data=df, x='index', y='Media', hue='Threads', hue_order=[0,1,2,4,8], ax = fig, legend=False, palette='deep')
fig.set(xlabel = 'Iterações', ylabel = 'Tempo (s)', title = f'Tempo de convolucação para uma imagem de {images["media"]}')
plt.legend(bbox_to_anchor=(1.02, 0.55), loc='upper left', borderaxespad=-.5)
plt.savefig(rf'./plots/teste.png')
"""