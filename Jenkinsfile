#!groovy

pipeline {
    agent none
    options {
        buildDiscarder(logRotator(numToKeepStr:'30'))
        timeout(time: 1500, unit: 'SECONDS')
    }
    stages {

        stage('BuildTest-Debug') {
            parallel {
                stage('FedoraGcc') {
                    agent { label "FedoraGcc" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """#!/bin/bash
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Foundation_Tester xml &&
                            ninja FoundationCoverage &&
                            export CODECOV_TOKEN="e946b501-1d85-4afa-9697-587d647917c7" &&
                            bash <(curl -s https://codecov.io/bash)
                        """ }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('MacOSSierra') {
                    agent { label "MacOSSierra" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """
                            export PATH='$PATH:/usr/local/bin/' &&
                            cmake -E env CXXFLAGS="-fno-var-tracking" cmake -G"Xcode" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            cmake --build . &&
                           ./Foundation_Tester xml
                        """ }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('Raspbian') {
                    agent { label "Raspbian" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """
                            hostname &&
                            export MEZZ_PACKAGE_DIR=/home/pi/Code/ &&
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Foundation_Tester xml
                         """ }
                    }
                    post {
                         always {
                             junit "build-debug/**/Mezz*.xml"
                         }
                    }
                }
                stage('UbuntuClang') {
                    agent { label "UbuntuClang" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """
                            cmake -E env CXXFLAGS="-fno-var-tracking" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja  &&
                            ./Foundation_Tester xml
                         """ }
                     }
                     post {
                         always {
                             junit "build-debug/**/Mezz*.xml"
                         }
                     }
                }
                stage('UbuntuEmscripten') {
                    agent { label "UbuntuEmscripten" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """
                            cmake -E env EMCC_DEBUG=2 CXXFLAGS="-v -fno-var-tracking -s WASM=1" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            node Foundation_Tester.js NoThreads
                        """ }
                    }
                    // Don't capture Emscripten logs, because it cannot make files
                }
                stage('UbuntuGcc') {
                    agent { label "UbuntuGcc" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-debug'
                        dir('build-debug') { sh """
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Foundation_Tester xml
                        """ }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('windows7Mingw32') {
                    agent { label "windows7Mingw32" }
                    steps {
                        checkout scm
                        bat 'if not exist "build-debug" mkdir build-debug'
                        dir('build-debug') {
                            bat 'cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF'
                            bat 'ninja'
                            bat 'Foundation_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('windows7Mingw64') {
                    agent { label "windows7Mingw64" }
                    steps {
                        checkout scm
                        bat 'if not exist "build-debug" mkdir build-debug'
                        dir('build-debug') {
                            bat 'cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF'
                            bat 'ninja'
                            bat 'Foundation_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
                stage('windows7msvc') {
                    agent { label "windows7msvc" }
                    steps {
                        checkout scm
                        bat 'if not exist "build-debug" mkdir build-debug'
                        dir('build-debug') {
                            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86_amd64 && cmake -G"Visual Studio 15 2017 Win64" .. -DCMAKE_BUILD_TYPE=DEBUG -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF'
                            bat 'cmake --build .'
                            bat 'Foundation_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-debug/**/Mezz*.xml"
                        }
                    }
                }
            }
        } // BuildTest-Debug

        stage('BuildTest-Release') {
            parallel {
                stage('FedoraGcc') {
                    agent { label "FedoraGcc" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """#!/bin/bash
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Foundation_Tester xml &&
                            ninja FoundationCoverage &&
                            export CODECOV_TOKEN="e946b501-1d85-4afa-9697-587d647917c7" &&
                            bash <(curl -s https://codecov.io/bash)
                        """ }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }
                stage('MacOSSierra') {
                    agent { label "MacOSSierra" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """
                            export PATH='$PATH:/usr/local/bin/' &&
                            cmake -E env CXXFLAGS="-fno-var-tracking" cmake -G"Xcode" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            cmake --build . &&
                           ./Foundation_Tester xml
                        """ }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }
                stage('Raspbian') {
                    agent { label "Raspbian" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """
                            hostname &&
                            export MEZZ_PACKAGE_DIR=/home/pi/Code/ &&
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Foundation_Tester xml
                         """ }
                    }
                    post {
                         always {
                             junit "build-release/**/Mezz*.xml"
                         }
                    }
                }
                stage('UbuntuClang') {
                    agent { label "UbuntuClang" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """
                            cmake -E env CXXFLAGS="-fno-var-tracking" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja  &&
                            ./Foundation_Tester xml
                         """ }
                     }
                     post {
                         always {
                             junit "build-release/**/Mezz*.xml"
                         }
                     }
                }
                stage('UbuntuEmscripten') {
                    agent { label "UbuntuEmscripten" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """
                            cmake -E env CXXFLAGS="-v -fno-var-tracking -s WASM=1" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            node Foundation_Tester.js NoThreads
                        """ }
                    }
                    // Don't capture Emscripten logs, because it cannot make files
                }
                stage('UbuntuGcc') {
                    agent { label "UbuntuGcc" }
                    steps {
                        checkout scm
                        sh 'mkdir -p build-release'
                        dir('build-release') { sh """
                            cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF &&
                            ninja &&
                            ./Foundation_Tester xml
                        """ }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }
                stage('windows7Mingw32') {
                    agent { label "windows7Mingw32" }
                    steps {
                        checkout scm
                        bat 'if not exist "build-release" mkdir build-release'
                        dir('build-release') {
                            bat 'cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF'
                            bat 'ninja'
                            bat 'Foundation_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }
                stage('windows7Mingw64') {
                    agent { label "windows7Mingw64" }
                    steps {
                        checkout scm
                        bat 'if not exist "build-release" mkdir build-release'
                        dir('build-release') {
                            bat 'cmake -E env CXXFLAGS="-fno-var-tracking-assignments" cmake -G"Ninja" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF'
                            bat 'ninja'
                            bat 'Foundation_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }
                stage('windows7msvc') {
                    agent { label "windows7msvc" }
                    steps {
                        checkout scm
                        bat 'if not exist "build-release" mkdir build-release'
                        dir('build-release') {
                            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86_amd64 && cmake -G"Visual Studio 15 2017 Win64" .. -DCMAKE_BUILD_TYPE=RELEASE -DMEZZ_BuildDoxygen=OFF -DMEZZ_CodeCoverage=OFF'
                            bat 'cmake --build .'
                            bat 'Foundation_Tester xml'
                        }
                    }
                    post {
                        always {
                            junit "build-release/**/Mezz*.xml"
                        }
                    }
                }
            }
        } // BuildTest-Release
    } // Stages

}
