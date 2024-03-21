# To prep, compile, and run on the host (from cFS directory above) as a normal user (best effort message queue depth and task priorities):
# 
# make SIMULATION=native prep
# make
# make install
# cd build/exe/cpu1/
# ./core-cpu1

FROM ubuntu:20.04 as cfs

# Set a non-interactive shell environment
ENV DEBIAN_FRONTEND=noninteractive

ARG USERNAME=niels

# Install cFS dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    python3-dev \
    python3-pip \
    python3-pyqt5 \
    python3-zmq \
    sudo \
    git && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Create a non-root user
RUN useradd -m -s /bin/bash ${USERNAME} && \
    echo "${USERNAME} ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers

# Clone the repository
RUN git clone --recursive https://github.com/nasa/cFS.git /home/${USERNAME}/cFS

# Change to the project directory
WORKDIR /home/${USERNAME}/cFS

# Copy in the default makefile and definitions
RUN cp cfe/cmake/Makefile.sample Makefile && cp -r cfe/cmake/sample_defs sample_defs && \
chown -R ${USERNAME}:niels /home/niels/cFS && \
su ${USERNAME} -c "make SIMULATION=native prep && make && make install"

FROM cfs as dev
ENV DEBIAN_FRONTEND=noninteractive

# Neovim deps
RUN apt-get update && apt-get install -y \
    ninja-build \
    gettext \
    libtool \
    libtool-bin \
    autoconf \
    automake \
    cmake \
    g++ \
    pkg-config \
    unzip \
    curl \
    doxygen && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Clone and build Neovim
RUN git clone https://github.com/neovim/neovim.git /neovim && \
    cd /neovim && \
    make CMAKE_BUILD_TYPE=RelWithDebInfo && \
    make install

# Set Neovim as the default editor
RUN update-alternatives --install /usr/bin/editor editor /usr/local/bin/nvim 60

# install dev env tools
RUN apt-get update && apt-get install -y \
    xclip \
    clangd \
    tree \
    clang-format \
    wget && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Set up user environment
USER ${USERNAME}
WORKDIR /home/${USERNAME}

# Install fzf
RUN git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf && \
    ~/.fzf/install --all

RUN echo "set -o vi\nalias vim='nvim'" >> .bashrc && \
    echo "source ~/.fzf.bash" >> .bashrc

RUN git clone https://github.com/nm47/dotfiles.git ~/.config/ && \
    nvim --headless "+Lazy! sync" +qa

CMD ["bash"]
