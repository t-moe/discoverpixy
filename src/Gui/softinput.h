//Arr = ref to String Array, ind_nospace = Index of touched String without empty strings, ind_spaced = real index
typedef void (*SOFTINPUT_CALLBACK)(char ** arr, unsigned char ind_nospace,unsigned char ind_spaced);	
extern void guiAddSoftInput(char ** arr, unsigned char h, SOFTINPUT_CALLBACK cb);
extern void guiRemoveSoftInput();
extern void guiUpdateSoftInput();

#define AUTO 0

extern char * allnums [12];
extern  char * allmonth [15];