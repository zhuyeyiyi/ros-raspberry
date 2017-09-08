
(cl:in-package :asdf)

(defsystem "imu_node-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Heading" :depends-on ("_package_Heading"))
    (:file "_package_Heading" :depends-on ("_package"))
  ))