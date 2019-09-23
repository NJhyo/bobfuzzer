# SystemCallMutator


### Data Structures

core/Image.hpp
```cpp
struct FileObject{
	std::string rel_path;
	uint8_t type;

	std::vector<BufferObject*> xattrs;
}

struct Image{
	std::vector<FileObject*> file_objs;
}
```

core/Utils.hpp
```cpp
struct BufferObject{
	uint8_t *buffer;
	uint32_t size;
]
```

core/Program.hpp
```cpp
struct Variable {
	std::string name;
	uint8_t type;
	uint32_t size;
	uint8_t* value;

	// kind: what this variable is (fd, mmap base, etc)
	uint8_t kind;
}

struct Arg {
	// an argument is either a variable (e.g., int x;)
	// or a literal constant (10)
	// if constant, put it here.
	// if variable, keep "index" here.
	// what is the "index"? program object has
	// a vector of variables. index is the index of
	// the variable in the vector.
	union {
		int64_t value;
		int64_t index;
	};
	uint8_t     is_variable;

	// For arg mutation, we don't want to mutate
	// things like fd. Let's mark those with a vector.
}

struct Syscall {
	int32_t nr; //./Constants.hpp 에서 참조해옴.
	std::vector<Arg*> args;
	// if we save return value, it must be a
	// variable;
	// if we do not care the return value,
	// by default it is -1.
	int64_t ret_index;
}

struct Program {
    // To make Program more extensible, we need a vector of indices.

	std::vector<Variable*> variables;
	std::vector<Syscall*> syscalls;

        // the list of various variable indices.
        //std::vector<std::vector<int64_t>> variable_indices;

	std::vector<int64_t> active_fds;
	std::vector<int64_t> active_file_fds;
	std::vector<int64_t> active_dir_fds;

        std::vector<int64_t> active_map_base_idx; //bases of mmap'd memory
        //std::vector<int64_t> active_map_size; // should match with above

	std::vector<FileObject*> avail_files;
	std::vector<FileObject*> avail_dirs;
	std::vector<FileObject*> avail_non_dirs;
	std::map<FileObject*, int64_t> file_paths; // store variables for path of all file objects

	uint32_t variable_cnt;
    	int64_t root_path_index;

	// by default, the first variable is
	// two two-page-size buffers for sending/receiving things from kernel
	static const int64_t src8192 = 0;
	static const int64_t dest8192 = 1;
	// v_2 .. v_n are variables for path
	static const int64_t path_start = 2;
 }
```

**core/SyscallMutator.hpp**
```cpp
class SyscallMutator {

    protected:
        Program* program;
        Syscall* original;
        Syscall* target;

    public:
        SyscallMutator(Program* program, Syscall* syscall = NULL) {
            this->program = program;
            this->target = syscall;
            if (syscall) {
                original = new Syscall(syscall->nr);
                *original = *target;
            } else original = NULL;
        }

        void setTarget(Syscall *syscall) {
            this->target = syscall;
            original = new Syscall(syscall->nr);
            *original = *target;
        }

        Syscall *getTarget() {
            return this->target;
        }

        virtual void releaseTarget() {
            //XXX: why is mine always the last?
            program->remove_last_syscall(this->target);
            this->target = NULL;
            if (original) {
                delete this->original;
                original = NULL;
            }
        }

        static SyscallMutator* create(Program* program, Syscall* syscall = NULL);
        static SyscallMutator* create_nr(Program* program, int32_t nr);
        virtual Syscall* createTarget(const ArgMap &argMap) = 0;

        virtual ~SyscallMutator() {
            if (original)
                delete original;
        }

        virtual uint64_t getStageMax(void) {
            assert(0 && "unimplemented!");
        }
        virtual uint32_t mutate(void) {
            assert(0 && "unimplemented!");
        }
        virtual uint32_t revert(void) {
            //assert(0 && "unimplemented!");
            assert(original != nullptr && target != nullptr);
            for (size_t i = 0; i < original->args.size(); i+=1) {
                *(target->args[i]) = *(original->args[i]);
            }
        }
        virtual uint32_t havoc(void) {
            //if(target) printf("havoc not implemented for %d\n",target->nr);
            //else printf("havoc called for null target\n");
            //assert(0 && "unimplemented!");
            //XXX: BAD: silently not mutating...
            //TODO
        }
        virtual bool done(void) {
            assert(0 && "unimplemented!");
        }
};
```
각 system call들의 mutator를 구현하기 위해서는 syscallMutator를 상속해야 한다.

Mutation과 관련된 method들은 virtual method이기 때문에 상속한 후 직접 구현해주어야 한다.

## about havoc
SyscallMutator.hpp
```cpp
uint32_t havoc(void) {
            target->args[2]->value = rand32(0, len.max + 1);
            return 0;
        }//ReadMutator

uint32_t havoc(void) {
     if (program->avail_files.empty())
       return 0;
           target->args[1]->index = program->file_paths[program->get_random_fobj()];
           return 0;
       }
```
각 함수의 특성에 따라 특정 parameter를 rand 함수를 이용하거나 get_random_xxx에서 avail_xxx 를 통하여 mutate 하는 것으로 확인할 수 있다.
```cpp
FileObject *get_random_fobj(void) {
		assert(avail_files.size() != 0);
		return avail_files[rand32(0, avail_files.size())];
	}
	FileObject* get_random_dir() {
		assert(avail_dirs.size() != 0);
		return avail_dirs[rand32(0, avail_dirs.size())];
	}
	FileObject* get_random_file() {
		assert(avail_non_dirs.size() != 0);
		return avail_non_dirs[rand32(0, avail_non_dirs.size())];
	}
  std::string Program::rand_path()
{
	FileObject *fobj = get_random_dir();
    auto it = file_paths.find(fobj);
    assert(it != file_paths.end());
	int64_t path_index = it->second;
	std::string ret = std::string((char *)(variables[path_index]->value));
	ret += "/" + random_string(8);
	return ret;
}
```
if there's a need to explain mutation stage, I'll add a source code later.

## Summarise parameters and variables for analysing source code
./core/afl-image-syscall/afl-fuzz -b btrfs -s fs/btrfs/btrfs_wrapper.so -e ./samples/evaluation/btrfs-00.image -S btrfs -y prog -i input -o output -m none -u 2 -- ./lkl/tools/lkl/btrfs-combined -t btrfs -p @@

**parameters**
```cpp
shm_name = "btrfs"//option b
in_dir = "input"//option i, at some point in_dir shifts to "input/queue"
out_dir = "output"//option o
sync_id = "btrfs"//option S
wrapper_file = "fs/btrfs/btrfs_wrapper.so"//option s
seed_file = "./samples/evaluation/btrfs-00.image"//option e
syscall_in_dir = "prog"//option y
-u : CPU#
```
**variables**
```cpp
use_argv = argv + optind;//parameters after @@
out_buf//contains the fuzzed input to the binary being tested
```

# initialising fsfuzzer
### afl-fuzz.c
**shared memory**
```cpp
shm_id : "shared memory"
static void setup_shm(void){
  .
  .
  shm_id = shmget(IPC_PRIVATE, MAP_SIZE, IPC_CREAT | IPC_EXCL | 0600);//allocates a System V shared memory segment
  .
  .
  trace_bits = shmat(shm_id, NULL, 0);//attach shared memory to process
  //return val is a address of shared memory attached to process
  .
  .
}
```

**load_seed_image()**
```cpp
//parsing image's meta data
fn = "input/seed.meta"//it is created in compress()
buffer = 'seed file's shared memory'
wrapper_compress(seed_file, buffer, fn);

//process of compress()
//1. load image from seed file
//2. get a super block from image_buffer
//3. parse the super block info using btrfs_parse_superblock, chuck_tree, root_tree, tree()
//4. write image_meta to input/seed.meata using release_metadata() *TODO* need to learn about extent in btrfs
```


**load_syscalls()**
```cpp
meta_fd = "input/seed.meta"
syscall_fd = "prog/open_read#"
testcase_fd = "input/open_read#"
lseek(meta_fd, 0, SEEK_SET);
sendfile(testcase_fd, meta_fd, NULL, meta_size);
sendfile(testcase_fd, syscall_fd, NULL, st.st_size);
//seems to concatenate meta_date and syscall
//copying files in prog to input.
//after loading syscalls, delete input/seed.meta
/*how files look like in input
 ---------------------------
|  meta_data  |  syscalls  |   ---> send to input dir
---------------------------
*/
```

assembling input file

# rand()
```cpp
/* Generate a random number (from 0 to limit - 1). This may
   have slight bias. */

static inline u32 UR(u32 limit) {

  if (unlikely(!rand_cnt--)) {

    u32 seed[2];

    ck_read(dev_urandom_fd, &seed, sizeof(seed), "/dev/urandom");

    srandom(seed[0]);
    rand_cnt = (RESEED_RNG / 2) + (seed[1] % RESEED_RNG);

  }

  return random() % limit;

}
//access to environmental noise collected from device drivers and other sources.
```
## fuzz(w...)
```cpp

fuzz_one(){
  if(fsfuzz_mode || ...)
    goto havoc_stage;
}
```
about general fuzzing
https://groups.google.com/forum/m/#!msg/afl-users/V8r7eq-NuvQ/ysgEhbeGAQAJ
