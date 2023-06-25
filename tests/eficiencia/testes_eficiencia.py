from subprocess import call

QTD_AMOSTRAGEM = 300


imagens = {'minuscula': 'pinguim',
           'pequena': 'firefox',
           'media': 'gato',
           'grande': 'abelha'
}

imagem_minuscula = 'pinguim'
imagem_pequena = 'firefox'
imagem_media = 'gato'
imagem_grande = 'abelha'

comando_concorrente = lambda imagem, num, tipo: f'./../../concorrente "./../../imagens_de_teste/{imagem}.png" "./../../imagens_alteradas/concorrente/{imagem}_{tipo}_{num}.png" {tipo}'
comando_sequencial = lambda imagem, tipo: f'./../../trab "./../../imagens_de_teste/{imagem}.png" "./../../imagens_alteradas/sequencial/{imagem}_{tipo}.png" {tipo}'

"""
cmds = {'minuscula': [f'{comando_concorrente(imagem_minuscula, 8, 1)} 3 8',
                      f'{comando_concorrente(imagem_minuscula, 4, 1)} 3 4', 
                      f'{comando_concorrente(imagem_minuscula, 2, 1)} 3 2', 
                      f'{comando_concorrente(imagem_minuscula, 1, 1)} 3 1', 
                      f'{comando_sequencial(imagem_minuscula, 1)} 3'],
        
        'pequena': [f'{comando_concorrente(imagem_pequena, 8, 1)} 3 8',
                    f'{comando_concorrente(imagem_pequena, 4, 1)} 3 4',
                    f'{comando_concorrente(imagem_pequena, 2, 1)} 3 2',
                    f'{comando_concorrente(imagem_pequena, 1, 1)} 3 1',
                    f'{comando_sequencial(imagem_pequena, 1)} 3'],
        
        'media': [f'{comando_concorrente(imagem_media, 8, 1)} 3 8',
                  f'{comando_concorrente(imagem_media, 4, 1)} 3 4',
                  f'{comando_concorrente(imagem_media, 2, 1)} 3 2',
                  f'{comando_concorrente(imagem_media, 1, 1)} 3 1',
                  f'{comando_sequencial(imagem_media, 1)} 3'],
        
        'grande': [f'{comando_concorrente(imagem_grande, 8, 1)} 3 8',
                   f'{comando_concorrente(imagem_grande, 4, 1)} 3 4',
                   f'{comando_concorrente(imagem_grande, 2, 1)} 3 2',
                   f'{comando_concorrente(imagem_grande, 1, 1)} 3 1',
                   f'{comando_sequencial(imagem_grande, 1)} 3']
        }"""

tipos = {0 : 'identidade',
         1 : 'media',
         2 : 'prewitt',
         3 : 'sobel'}

for tipo_index in tipos:
    if tipo_index == 2:
        imagens = {'minuscula': 'hugo_nobrega_ponto_com',
           'pequena': 'chuu_flamenguista',
           'media': 'scooby_doo',
           'grande': 'TRIBE'
        }
    for tamanho in imagens:
        threads = 1
        while threads < 9:
            for i in range(QTD_AMOSTRAGEM):
                call(f'{comando_concorrente(imagens[tamanho], threads, tipo_index)} 3 {threads} >> {tipos[tipo_index]}/imagem_{tamanho}/{threads}_threads.csv', shell=True)
                if threads == 1:
                    call(f'{comando_sequencial(imagens[tamanho], tipo_index)} 3 >> {tipos[tipo_index]}/imagem_{tamanho}/sequencial.csv', shell=True)
                    
            call(f'echo "{comando_concorrente(imagens[tamanho], threads, tipo_index)} 3 {threads} >> {tipos[tipo_index]}/imagem_{tamanho}/{threads}_threads.txt" >> {tipos[tipo_index]}/imagem_{tamanho}/comandos_concorrentes.txt', shell=True)
            call(f'echo "{comando_sequencial(imagens[tamanho], tipo_index)} 3 >> {tipos[tipo_index]}/imagem_{tamanho}/sequencial.txt" >> {tipos[tipo_index]}/imagem_{tamanho}/comandos_sequenciais.txt', shell=True)
            threads *= 2