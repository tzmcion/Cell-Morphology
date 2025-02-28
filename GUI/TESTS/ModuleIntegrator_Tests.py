from optimization.ModuleIntegration import ModuleIntegration
import os
import time

def test_integrator_paths():
    OPTS = os.getcwd() + "/GUI/TESTS/FILE_SAMPLES/sample_options.option"
    IMAGES = os.getcwd() + "/GUI/TESTS/FILE_SAMPLES/paths.txt"
    OUT = os.getcwd() + "/GUI/TESTS/FILE_SAMPLES/TEST_PATHS"
    integrator = ModuleIntegration()
    integrator.run_subprocess([OPTS,IMAGES,OUT])
    time.sleep(0.5)
    integrator.kill_subprocess()
    assert os.path.exists(OUT + "/temp/cropped_bg.jpg") == True