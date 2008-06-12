# tar cz . | ssh joey@neuralyte.org eval "cd ircbot && tar xz"
# tar cz bin lib *.sh | ssh joey@neuralyte.org eval "cd ircbot && tar xz"
tar cz bin *.sh DO_NOT_* | ssh joey@neuralyte.org eval "cd ircbot && tar xzv"
