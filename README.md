# LFTC

#compile classifier:
g++ -o classifier classifier.cpp

#flex:

##compile lex
lex lexer.lex

##compile lex c output
gcc lex.yy.c

##feed source code text into lex exe
./a.out < source.mlp

##feed lex parsed atoms into classifier (to generate FIP and TS)
./a.out < source.mlp | xargs --null ./classifier

