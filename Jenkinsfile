pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
      label 'docker-host-dev'
    }
  }
  options {
    buildDiscarder(logRotator(numToKeepStr: '30', artifactNumToKeepStr: '30'))
    timeout(time: 1, unit: 'HOURS')
  }

  stages {
    stage('Git') { steps {
      sh "git submodule update --init"
    } }
    stage('CMake') { steps {
      dir("${env.WORKSPACE}/QBeat/build") {
        sh "cmake -DCMAKE_INSTALL_PREFIX=${env.WORKSPACE}/QBeat/install ${env.WORKSPACE}/QBeat/"
      }
    } }
    stage('Build') {
      steps {
        dir("${env.WORKSPACE}/QBeat/build") {
          sh '''make install'''
        }
      }
    }
    stage('Artifacts') {
      steps {
          sh "tar -C ${env.WORKSPACE}/QBeat/install/ -cvzf ${env.WORKSPACE}/QBeat.tar.gz ."
archiveArtifacts artifacts: 'QBeat.tar.gz', fingerprint: true, onlyIfSuccessful: true
      }
    }
  }

  post {
    always {
emailext attachLog: true, body: '''${DEFAULT_CONTENT}''', compressLog: true, recipientProviders: [[$class: 'CulpritsRecipientProvider']], subject: '${DEFAULT_SUBJECT}', to: '${DEFAULT_RECIPIENTS}'
    }
  }
}

