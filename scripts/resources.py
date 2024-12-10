import shutil

resource_path = '../res/'
dat_gameboy_res_path = '../dat-gameboy/res'
dat_gameboy_tests_res_path = '../dat-gameboy-test/res'


def main():
    shutil.copytree(resource_path, dat_gameboy_res_path, dirs_exist_ok=True)
    shutil.copytree(resource_path, dat_gameboy_tests_res_path, dirs_exist_ok=True)
    
    
if __name__ == '__main__':
    main()
