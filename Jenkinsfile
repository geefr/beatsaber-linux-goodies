pipeline {
  agent {
    dockerfile {
      filename 'source/Dockerfile'
      label 'docker-host-dev'
    }
  }
  options {
    buildDiscarder(logRotator(numToKeepStr: '30', artifactNumToKeepStr: '30'))
    checkoutToSubdirectory('source')
    timeout(time: 1, unit: 'HOURS')
  }

  stages {
    stage('CMake') { steps {
      dir("${env.WORKSPACE}/build") {
        sh '''cmake -DCMAKE_INSTALL_PREFIX=../install ../source/QBeat'''
      }
    } }
    stage('Build') {
      steps {
        dir("${env.WORKSPACE}/build") {
          sh '''make install'''
        }
      }
    }
    stage('Artifacts') {
      steps {
        dir("${env.WORKSPACE}/install") {
          sh '''tar -cvzf ../QBeat-Debian.tar.gz ./*'''
archiveArtifacts artifacts: '../QBeat-Debian.tar.gz', fingerprint: true, onlyIfSuccessful: true
        }      
      }
    }
  }

  post {
    always {
emailext attachLog: true, body: '''${DEFAULT_CONTENT}''', compressLog: true, recipientProviders: [[$class: 'CulpritsRecipientProvider']], subject: '${DEFAULT_SUBJECT}', to: '${DEFAULT_RECIPIENTS}'
    }
  }
}

