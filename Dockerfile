FROM ubuntu:18.04

MAINTAINER tobitti0

RUN set -xe && \
    apt-get update && \
    apt-get -y install software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y aptitude && \
    aptitude update && \
    aptitude install -y \
        wget build-essential automake autoconf git libtool libvorbis-dev \
        libass-dev libfreetype6-dev libsdl2-dev libva-dev libvdpau-dev \
        libxcb1-dev libxcb-shm0-dev libxcb-xfixes0-dev \
        mercurial libnuma-dev texinfo zlib1g-dev \
        cmake qtbase5-dev checkinstall gcc-9 g++-9 curl \
        ninja-build \
        nasm libsdl2-dev libx264-dev libx265-dev 

RUN sed -Ei 's/^# deb-src /deb-src /' /etc/apt/sources.list && \
    apt-get update && \
    apt-get -y build-dep ffmpeg

#AviSynth+
RUN cd /tmp/ && \
    git clone git://github.com/AviSynth/AviSynthPlus.git && \
    cd AviSynthPlus && \
    mkdir avisynth-build && \
    cd avisynth-build && \
    CC=gcc-9 CXX=gcc-9 LD=gcc-9 cmake ../ -G Ninja && \
    ninja && \
    checkinstall --pkgname=avisynth --pkgversion="$(grep -r \
    Version avs_core/avisynth.pc | cut -f2 -d " ")-$(date --rfc-3339=date | \
    sed 's/-//g')-git" --backup=no --deldoc=yes --delspec=yes --deldesc=yes \
    --strip=yes --stripso=yes --addso=yes --fstrans=no --default ninja install

RUN cd /tmp/ && \
    git clone git://git.ffmpeg.org/ffmpeg.git && \
    cd /tmp/ffmpeg && \
    ./configure \
      --enable-gpl\
      --enable-version3 \
      --disable-doc \
      --disable-debug \
      --enable-pic \
      --enable-avisynth \
      --enable-libx264 \
      --enable-libx265 \
      --enable-nonfree \
      && \
    make -j$(nproc) && \
    checkinstall --pkgname=ffmpeg --pkgversion="7:$(git rev-list \
    --count HEAD)-g$(git rev-parse --short HEAD)" --backup=no --deldoc=yes \
    --delspec=yes --deldesc=yes --strip=yes --stripso=yes --addso=yes \
    --fstrans=no --default

#FFMS2
RUN cd /tmp &&\
    git clone git://github.com/ffms/ffms2.git && \
    cd ffms2 && \
    PKG_CONFIG_PATH=$HOME/ffavx_build/lib/pkgconfig \
    CPPFLAGS="-I/usr/local/include/avisynth" \
    ./autogen.sh --enable-shared --enable-avisynth && \
    make -j$(nproc) && \
    checkinstall --pkgname=ffms2 --pkgversion="1:$(./version.sh)-git" \
    --backup=no --deldoc=yes --delspec=yes --deldesc=yes --strip=yes --stripso=yes \
    --addso=yes --fstrans=no --default

#install delogo
ADD src /tmp/src
RUN cd /tmp/src &&\
    make CC=gcc-9 CXX=g++-9 LD=gcc-9 &&\
    mv libdelogo.so /usr/local/lib/avisynth


