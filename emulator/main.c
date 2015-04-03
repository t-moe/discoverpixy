
extern void qt_init(int argc, char* argv[]); 
extern int qt_execute();


int main(int argc, char* argv[]) {
	qt_init(argc,argv);
	return qt_execute();
}
