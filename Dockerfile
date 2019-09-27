FROM debian:10
LABEL project="beatsaber-linux-goodies"
LABEL MAINTAINER Gareth Francis (gfrancis.dev@gmail.com)
RUN apt-get update && apt-get install -y cmake gcc g++ git
RUN apt-get install -y libz-dev libminizip-dev qtdeclarative5-dev qtbase5-dev

