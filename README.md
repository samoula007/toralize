# Usage
- Pass to the toralize script a command you want to run through the tor proxy
- e.g:
  - ./path/to/script/toralize <command> <command args>


# Steps to run the project
0. install docker and run the daemon (e.g. open the app on mac); no need to have an account
1. run "make all"
2. example command: ./usr/local/bin/toralize curl http://mittip.se
3. run "exit"
4. run "make stop"
