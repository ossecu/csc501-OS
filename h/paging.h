/* paging.h */

typedef unsigned int	 bsd_t;

/* Structure for a page directory entry */

typedef struct {

  unsigned int pd_pres	: 1;		/* page table present?		*/
  unsigned int pd_write : 1;		/* page is writable?		*/
  unsigned int pd_user	: 1;		/* is use level protection?	*/
  unsigned int pd_pwt	: 1;		/* write through cachine for pt?*/
  unsigned int pd_pcd	: 1;		/* cache disable for this pt?	*/
  unsigned int pd_acc	: 1;		/* page table was accessed?	*/
  unsigned int pd_mbz	: 1;		/* must be zero			*/
  unsigned int pd_fmb	: 1;		/* four MB pages?		*/
  unsigned int pd_global: 1;		/* global (ignored)		*/
  unsigned int pd_avail : 3;		/* for programmer's use		*/
  unsigned int pd_base	: 20;		/* location of page table?	*/
} pd_t;

/* Structure for a page table entry */

typedef struct {

  unsigned int pt_pres	: 1;		/* page is present?		*/
  unsigned int pt_write : 1;		/* page is writable?		*/
  unsigned int pt_user	: 1;		/* is use level protection?	*/
  unsigned int pt_pwt	: 1;		/* write through for this page? */
  unsigned int pt_pcd	: 1;		/* cache disable for this page? */
  unsigned int pt_acc	: 1;		/* page was accessed?		*/
  unsigned int pt_dirty : 1;		/* page was written?		*/
  unsigned int pt_mbz	: 1;		/* must be zero			*/
  unsigned int pt_global: 1;		/* should be zero in 586	*/
  unsigned int pt_avail : 3;		/* for programmer's use		*/
  unsigned int pt_base	: 20;		/* location of page?		*/
} pt_t;

typedef struct{
  unsigned int pg_offset : 12;		/* page offset			*/
  unsigned int pt_offset : 10;		/* page table offset		*/
  unsigned int pd_offset : 10;		/* page directory offset	*/
} virt_addr_t;

typedef struct{
  int bs_status;			/* MAPPED or UNMAPPED		*/
  int bs_pid;				/* process id using this slot   */
  int bs_vpno;				/* starting virtual page number */
  int bs_npages;			/* number of pages in the store */
  int bs_sem;				/* semaphore mechanism ?	*/
  int bs_page_validbit[128]; //says if there are any empty pages if this value is 1 and make all of them 0 in xmunmap
} bs_map_t;

typedef struct{
  int fr_status;			/* MAPPED or UNMAPPED		*/
  int fr_pid;				/* process id using this frame  */
  int fr_vpno;				/* corresponding virtual page no*/
  int fr_refcnt;			/* reference count		*/
  int fr_type;				/* FR_DIR, FR_TBL, FR_PAGE	*/
  int fr_dirty;       //should be 0 when free and 1 when occupied
  void *cookie;				/* private data structure	*/
  unsigned long int fr_loadtime;	/* when the page is loaded 	*/
}fr_map_t;

extern bs_map_t bsm_tab[];
extern fr_map_t frm_tab[];
/* Prototypes for required API calls */
SYSCALL xmmap(int, bsd_t, int);
SYSCALL xunmap(int);

//OS proj 3 modify
extern int page_replace_policy;

//OS proj 3 modify
//include frame.c implemented functions in .h file for other .c files and functions to access
SYSCALL free_frm(int i); //free the i frame passed as FBR + index
SYSCALL int get_frm();
void enable_paging();
SYSCALL init_bsm();
SYSCALL init_frm();
SYSCALL bsm_map(int pid, int vpno, int store, int npages);
unsigned long init_pagedir(int proc);
void write_cr3(unsigned long n);
unsigned long read_cr2(void);
extern int global_pagetable_fr[];
extern unsigned long global_pagetable_addr[];
void share_global_tables(unsigned long pdbr_proc);
void map_frame_to_proc_virtpage(int frameno, int proc, unsigned long virt_page, int frame_type);
SYSCALL int ispagefaultaadr_mapped_bsm_lookup(int pid, long vaddr);
SYSCALL srpolicy(int policy);
SYSCALL int get_bsm();//returns free BS if available


/* given calls for dealing with backing store */

int get_bs(bsd_t, unsigned int);
SYSCALL release_bs(bsd_t);
SYSCALL read_bs(char *, bsd_t, int);
SYSCALL write_bs(char *, bsd_t, int);

#define NBPG		4096	/* number of bytes per page	*/
#define FRAME0		1024	/* zero-th frame		*/

//default 3072 frames --> 1024+3072=4096=16M
//#define NFRAMES 	3072	/* number of frames		*/
//#define NFRAMES 	8	/* number of frames		*/
#define NFRAMES   1024

#define BSM_UNMAPPED	0
#define BSM_MAPPED	1

#define FRM_UNMAPPED	0
#define FRM_MAPPED	1

#define FR_PAGE		0
#define FR_TBL		1
#define FR_DIR		2

#define FIFO		3
#define LRU		4

//#define MAX_ID          9              /* You get 10 mappings, 0 - 9 */
//OS proj 3 modify
#define MAX_ID 16 //maximum 16 backing stores

#define BACKING_STORE_BASE	0x00800000
#define BACKING_STORE_UNIT_SIZE 0x0080000
