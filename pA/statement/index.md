# 忘卻疑惑

\begin{figure}[h]
\centering
\includegraphics[width=3in]{main.jpg}
\caption{超かぐや姫！}
\end{figure}


輝夜時常會拉著彩葉玩各種遊戲，比如說幾A幾B。輸的就要買鬆餅請客。

但彩葉有時猜到後來總覺得不對勁，懷疑粗枝大葉的輝夜根本把原本想的數字忘了。

雖然彩葉有把當時猜的過程記錄下來，可是他的行程太滿沒空去仔細比對。為了不讓他過勞，你能幫幫他嗎？

玩法是輝夜會選擇一組 $6$ 位數字，位數可重複、允許開頭 $0$。

彩葉會猜 $n$ 次，每次猜一組 $6$ 位數字，輝夜就必須回答幾 A 幾 B。設答案為 $S$ 猜測為 $T$ 時，定義 A 與 B 如下：

- A 為存在幾組位置相同、數字相同的位數，$S_i=T_i$
- B 為存在幾組數字相同、位置相異的位數，$i\neq j$ 且 $S_i=T_j$
- 若有數字 A 和 B 同時成立時，A 優先；每位數字只能用於最多一組 B。

\clearpage

\begin{figure}[h]
\centering
\includegraphics[width=3in]{main2.jpg}
\caption{超かぐや姫！}
\end{figure}

例如對 $S=$`111234` 猜 $T=$`122337` 結果為 $2$ A $1$ B：

- $S_1=T_1$ 且 $S_5=T_5$ 共 $2$ A
- $S_4=T_2$ 共 $1$ B。$S_4=T_3$ 但 $S_4$ 只能配對一次，$S_5=T_4$ 但 $S_5$ 優先成立 A。

根據這 $n$ 次猜測的結果判斷輝夜想的數字是什麼。如果無法特定，輸出 `Ambiguous`；如果輝夜中途把數字忘了打算偷偷糊弄過去，則輸出 `Impossible`。

\clearpage

## 輸入
輸入第一行為正整數 $n$  
接下來 $n$ 行，每行一次猜測六位數數字 $S$ 以及非負整數 $x, y$ 代表對該猜測回答 $x$ A $y$ B

## 輸出
輸出一行代表判斷結果。若能確定輝夜想的數字，輸出那六位數字。  
如果無法特定，輸出 `Ambiguous`。  
如果輝夜中途把數字忘了打算偷偷糊弄過去，則輸出 `Impossible`。

## 輸入限制
 - $n\leq 255$
 - $S$ 必為六位數數字

## 子任務
\subtasks

\clearpage

## 範例輸入一
\testfile{0-01.in}

## 範例輸出一
\testfile{0-01.out}

## 範例輸入二
\testfile{0-02.in}

## 範例輸出二
\testfile{0-02.out}

## 範例輸入三
\testfile{0-03.in}

## 範例輸出三
\testfile{0-03.out}
