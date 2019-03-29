
struct shprocs{
	struct proc *proc;
	uint mapped;
};
struct shpage{
	KEY key;	             //key
	struct shprocs shproc[16];  //poies diergasies tin exoun mapped kai pou
	int sharing;
	void  *pa;
};
