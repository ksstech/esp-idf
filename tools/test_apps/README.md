# Test Apps

This directory contains a set of ESP-IDF projects to be used as tests only, which aim to exercise various
configuration of components to check completely arbitrary functionality should it be building only, executing under
various conditions or combination with other components, including custom test frameworks.

The test apps are not intended to demonstrate the ESP-IDF functionality in any way.

# Test Apps projects

Test applications are treated the same way as ESP-IDF examples, so each project directory shall contain
* Build recipe in cmake and the main component with app sources
* Configuration files, `sdkconfig.ci` and similar (see below)
* Test executor in `ttfw_idf` format if the project is intended to also run tests (otherwise the example is build only)
  - test file in the project dir must end with `_test.py`, by should be named  `app_test.py`
  - test cases shall be decorated with `@ttfw_idf.idf_custom_test(env_tag="...")`


# CI Behavior

## Configuration Files

For each project in test_apps (and also examples):

* If a file `sdkconfig.ci` exists then it's built as the `default` CI config (same as if this file was named `sdkconfig.ci.default`)
* If any files `sdkconfig.ci.<CONFIG>` exist then these are built as alternative configs, with the specified `<CONFIG>` name.

* By default, every CI configurations is built for every target SoC (an `m * n` configuration matrix). However if any `sdkconfig.ci` file contains a line of the form `CONFIG_IDF_TARGET="targetname"` then that CI config is only built for that one target.
* Each configuration is also built with the contents of any `sdkconfig.defaults` file or a file named `sdkconfig.defaults.<TARGET>` appended. (Same as a normal ESP-IDF project build.)

## Test Execution

If an example test or test app test supports more targets than just `ESP32`, then the `app_test.py` file needs to specify the list of supported targets in the test decorator. For example:

```
@ttfw_idf.idf_example_test(env_tag='Example_GENERIC', target=['esp32', 'esp32s2'])
def test_app_xyz(env, extra_data):
```

If the app test supports multiple targets but you only want some of these targets to be run automatically in CI, the list can be further filtered down by adding the `ci_target` list:

```
@ttfw_idf.idf_example_test(env_tag='Example_GENERIC', target=['esp32', 'esp32s2'], ci_target=['esp32'])
def test_app_xyz(env, extra_data):
```

(If no ci_target list is specified, all supported targets will be tested in CI.)

# Test Apps layout

The test apps should be grouped into subdirectories by category. Categories are:
* `protocols` contains test of protocol interactions.
* `network` contains system network tests
* `system` contains tests on the internal chip features, debugging and development tools.
* `security` contains tests on the chip security features.

# Test Apps local execution

* Append relevant `sdkconfig.ci.<CONFIG>` to the sdkconfig for the configuration under test
* Run `idf.py menuconfig` to configure local project attributes
* Run `idf.py build` to build the test app
* Run `python app_test.py` to run the test locally
