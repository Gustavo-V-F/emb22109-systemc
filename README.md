# emb22109-systemc
Projeto de um processador da arquitetura [Neander](https://www.inf.ufrgs.br/arq/wiki/doku.php?id=neander) em [SystemC](https://www.accellera.org/downloads/standards/systemc). A sua especificação é fornecida pela [máquina de estados desenvolvida em Yakindu Statecharts Tools](yakindu/neander_uc.ysc) e pelo arquivo [emb22109-proj\_systemc.pdf](emb22109-proj_systemc.pdf).

São executadas todas as instruções disponíveis (apresentadas abaixo) ao executar o programa compilado, e a sua simulação foi conferida com o auxílio do simulador
[wneander](https://www.inf.ufrgs.br/arq/wiki/lib/exe/fetch.php?media=wneander.zip). Ao final elas podem ser comparadas
por meio do arquivo [testbench.txt](wneander/testbench.txt), em que seu resultado deve ser igual ao obtido da
execução do projeto.

```C
        [...]
		sc_uint<8> text[] = { LDA, 128, ADD, 129, JZ, 8, ADD, 129, JN, 12, ADD,
				129, JN, 16, ADD, 128, AND, 130, OR, 131, STA, 132, NOT, JMP,
				27, JMP, 30, STA, 133, NOP, HLT };
		sc_uint<8> data[] = { 1, -1, 7, 14, 0, 0 };
        [...]
```
