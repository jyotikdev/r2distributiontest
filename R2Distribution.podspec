Pod::Spec.new do |s|
  pod_name = 'R2Distribution'

  framework_filename = "#{pod_name}.xcframework"
  version_number = "v2.0.0-beta.1"

  s.name = pod_name
  s.version = '2.0.0-beta1'
  s.license = 'Developer license'
  s.homepage = 'https://github.com/jyotikdev/r2distributiontest'
  s.authors = 'Jyoti & Tamer'

  s.summary = 'A testing framework'

  s.ios.deployment_target = '13.0'

  s.source = { :git => "https://github.com/jyotikdev/r2distributiontest/releases/download/v2.0.0-beta.1/R2Distribution.xcframework.zip"}

  s.vendored_frameworks = "R2Distribution.xcframework"
  
  s.prepare_command = <<-CMD
                      unzip R2Distribution.xcframework.zip
                      CMD

end