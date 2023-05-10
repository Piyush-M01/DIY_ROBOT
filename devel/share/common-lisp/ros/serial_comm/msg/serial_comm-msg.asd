
(cl:in-package :asdf)

(defsystem "serial_comm-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Ticks" :depends-on ("_package_Ticks"))
    (:file "_package_Ticks" :depends-on ("_package"))
  ))