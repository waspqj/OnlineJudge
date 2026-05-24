oj_server: oj_server.cc
	g++ -g $^ -o $@ -ljsoncpp -lctemplate -lmysqlclient 
# -I../ctemplate-master/src/
.PHONY: clean
clean:
	rm oj_server