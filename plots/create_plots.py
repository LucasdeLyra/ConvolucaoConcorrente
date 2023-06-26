import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
pd.set_option('display.max_rows', 5000)
COLUMNS = ['read', 'pre_processing', 'create_kernel', 'convolution', 'write']
#https://stackoverflow.com/questions/35634238/how-to-save-a-pandas-dataframe-table-as-a-png



tipos = {0 : 'identidade',
         1 : 'media',
         2 : 'prewitt',
         3 : 'sobel'}


def merge_all_csv_from_directory(directory):
    final_df = 0

    final_df = pd.read_csv(rf'{directory}/sequencial.csv', sep=';', header=None, names=COLUMNS)
    
    final_df['tempo_total'] = final_df.sum(axis=1)  
    final_df = final_df.reset_index()
    num_threads = ['Sequencial']*len(final_df)
    final_df['Threads'] = num_threads
    
    i = 1
    while i <= 8:
        df = pd.read_csv(rf'{directory}/{i}_threads.csv', sep=';', header=None, names=COLUMNS)
        df['tempo_total'] = df.sum(axis=1)
        df = df.reset_index()
        num_threads = [i]*len(df)
        df['Threads'] = num_threads
        final_df= final_df.append(df, ignore_index=True)
        i *= 2
    
    return final_df


def get_altogether_convolution_plot(column_name, title):
    imagens = {'minuscula': '236x236', 
          'pequena': '615x407', 
          'media': '2480x1365', 
          'grande': '5600x3200'}
    for tipo_index in tipos:
        
        if tipo_index == 2:
            imagens = {'minuscula': '250x250',
            'pequena': '401x403',
            'media': '1920x1080',
            'grande': '4000x3000'
            }
            
        for image_size in imagens:
            dir = rf'./../tests/eficiencia/{tipos[tipo_index]}/imagem_{image_size}'
            merged_csv = merge_all_csv_from_directory(dir)
            
            fig = sns.relplot(data=merged_csv, x='index', y=f'{column_name}', hue='Threads', hue_order=['Sequencial',1,2,4,8], aspect=1.7, palette='deep')
            fig.set(xlabel = 'Iterações', ylabel = 'Tempo (s)', title = f'{title} para uma imagem de {imagens[image_size]}')
            fig.savefig(rf'./plots/{tipos[tipo_index]}/{tipos[tipo_index]}_{column_name}_{image_size}.png')
            plt.close()
            
            """fig = sns.relplot(data=merged_csv, x='index', y=f'{column_name}', col='Threads', aspect=0.5)
            fig.set(xlabel = 'Iterações', ylabel = 'Tempo (s)')
            fig.savefig(rf'./plots/{column_name}_{kernel}_{image_size}_separado.png')
            plt.close()"""

def all_in_one_plot(column_name, title):
    f, ax = plt.subplots(4,4)
    f.set_figwidth(30)
    f.set_figheight(20)
    f.suptitle(title)
    
    imagens = {'minuscula': '236x236', 
          'pequena': '615x407', 
          'media': '2480x1365', 
          'grande': '5600x3200'}
    
    for tipo_index in tipos:
        
        if tipo_index == 2:
            imagens = {'minuscula': '250x250',
            'pequena': '401x403',
            'media': '1920x1080',
            'grande': '4000x3000'
            }
            
        for j, image_size in enumerate(imagens):
            dir = rf'./../tests/eficiencia/{tipos[tipo_index]}/imagem_{image_size}'
            merged_csv = merge_all_csv_from_directory(dir)

            fig  = sns.scatterplot(data=merged_csv, x='index', y=f'{column_name}', hue='Threads', hue_order=['Sequencial',1,2,4,8], legend=False, palette='deep', ax=ax[tipo_index][j])
            fig.set(xlabel = 'Iterações', ylabel = 'Tempo (s)', title = f'{tipos[tipo_index]} - {imagens[image_size]}')
            #f.legend(bbox_to_anchor=(1.02, 0.55), loc='upper left', borderaxespad=-.5)
    return f


def all_in_one_plot_with_subtitles(column_name, title):
    fg = plt.figure(constrained_layout=True)
    fg.suptitle(title, fontsize=22, fontweight='heavy')
    fg.set_figwidth(30)
    fg.set_figheight(20)
    
    imagens = {'minuscula': '236x236', 
          'pequena': '615x407', 
          'media': '2480x1365', 
          'grande': '5600x3200'}
    subfigs = fg.subfigures(nrows=4, ncols=1)
    for tipo_index in tipos:
        
        if tipo_index == 2:
            imagens = {'minuscula': '250x250',
            'pequena': '401x403',
            'media': '1920x1080',
            'grande': '4000x3000'
            }
            
        subfigs[tipo_index].suptitle(f'{tipos[tipo_index].capitalize()}', fontsize=16, fontweight='semibold')
        axs = subfigs[tipo_index].subplots(nrows=1, ncols=4)
        for j, image_size in enumerate(imagens):
            
            dir = rf'./../tests/eficiencia/{tipos[tipo_index]}/imagem_{image_size}'
            merged_csv = merge_all_csv_from_directory(dir)

            sns.scatterplot(data=merged_csv, x='index', y=f'{column_name}', hue='Threads', hue_order=['Sequencial',1,2,4,8], legend=False, palette='deep', ax=axs[j]).set(xlabel = 'Iterações', ylabel = 'Tempo (s)', title = f'{imagens[image_size]}')
            #fig.set(xlabel = 'Iterações', ylabel = 'Tempo (s)', title = f'{imagens[image_size]}')

            axs[j].plot()
            #f.legend(bbox_to_anchor=(1.02, 0.55), loc='upper left', borderaxespad=-.5)
    return fg

"""all_in_one_plot_with_subtitles('convolution', 'Tempo de convolução').savefig(rf'./plots/all_in_one_convolution.png')
get_altogether_convolution_plot('read', 'Tempo de leitura')
get_altogether_convolution_plot('pre_processing', 'Tempo de pré-processamento')
get_altogether_convolution_plot('write', 'Tempo de escrita')
get_altogether_convolution_plot('tempo_total', 'Tempo total')"""


def print_average(column):
    imagens = {'minuscula': '236x236', 
        'pequena': '615x407', 
        'media': '2480x1365', 
        'grande': '5600x3200'}
    df_median = pd.DataFrame() 
    for tipo_index in tipos:
        
        if tipo_index == 2:
            imagens = {'minuscula': '250x250',
            'pequena': '401x403',
            'media': '1920x1080',
            'grande': '4000x3000'
            }
        df_mean = pd.DataFrame() 
        aux = pd.DataFrame() 
        for image_size in imagens:
            dir = rf'./../tests/eficiencia/{tipos[tipo_index]}/imagem_{image_size}'
            merged_csv = merge_all_csv_from_directory(dir)

            df_mean = pd.concat([df_mean, pd.DataFrame({f'Média da imagem {image_size}': merged_csv.groupby('Threads')[f'{column}'].mean()})], axis=1)
            aux['Tipo do efeito'] = tipos[tipo_index].capitalize()
            aux = pd.concat([aux, pd.DataFrame({f'Mediana da imagem {image_size}': merged_csv.groupby('Threads')[f'{column}'].median()})], axis=1)
            
            #aux = pd.concat([aux, pd.DataFrame([{f'Tipo do efeito': tipos[tipo_index]}])], axis=1)
        
        df_median = pd.concat([df_median, aux.rename(columns={'index': 'Threads'})], axis=0)
        
        fig, ax = plt.subplots(figsize=(12, 2))
        ax.xaxis.set_visible(False)
        ax.yaxis.set_visible(False)
        ax.set_frame_on(False)
        tabla = pd.plotting.table(ax, df_mean, loc='upper right', colWidths=[0.2]*len(df_mean.columns), cellColours=[['lightcyan']*4, ['paleturquoise']*4, ['lightcyan']*4, ['paleturquoise']*4, ['lightcyan']*4], colColours=['gainsboro']*4, rowColours=['whitesmoke','gainsboro', 'whitesmoke','gainsboro', 'whitesmoke'], cellLoc='left')
        tabla.auto_set_font_size(False)
        tabla.set_fontsize(9)
        tabla.scale(1.2, 1.2)

        plt.savefig(f'./plots/tabelas/{tipos[tipo_index]}_{column}_mean.png', transparent=True)
        
    
    fig, ax = plt.subplots(figsize=(15, 12))
    ax.xaxis.set_visible(False)
    ax.yaxis.set_visible(False)
    ax.set_frame_on(False)
    tabla = pd.plotting.table(ax, df_median, loc='center right', colWidths=[0.2]*len(df_median.columns), cellColours=[['lightcyan']*5,['paleturquoise']*5]*10, colColours=['gainsboro']*5, rowColours=['whitesmoke','gainsboro']*10, cellLoc='left')
    tabla.auto_set_font_size(False)
    tabla.auto_set_column_width([0,1,2,3,4,5])
    tabla.set_fontsize(9)
    tabla.scale(1.3, 1.3)
    plt.savefig(f'./plots/tabelas/{column}_median.png', transparent=True)
print_average('convolution')
print_average('tempo_total')
"""
df = df.merge(df2, on='Threads', how='left')

fig  = sns.scatterplot(data=df, x='index', y='convolution', hue='Threads', hue_order=[0,1,2,4,8], legend='auto', palette='deep')
sns.lineplot(data=df, x='index', y='Media', hue='Threads', hue_order=[0,1,2,4,8], ax = fig, legend=False, palette='deep')
fig.set(xlabel = 'Iterações', ylabel = 'Tempo (s)', title = f'Tempo de convolucação para uma imagem de {imagens["media"]}')
plt.legend(bbox_to_anchor=(1.02, 0.55), loc='upper left', borderaxespad=-.5)
plt.savefig(rf'./plots/teste.png')
"""