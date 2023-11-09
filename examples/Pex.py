import threading
import typing
import logging

class va_pack:
  def __init__(self, *args, **kwargs) -> None:
    self._args = args
    self._kwargs = kwargs

  def invoke(self, fn, *prior_args):
    return fn(*prior_args, *self._args, **self._kwargs)

class task_base:
  def __init__(self, executeFn):
    self._next = None
    self._execute = executeFn

  def execute(self):
    self._execute(self)

class loop_sched_operation(task_base):
  def __init__(self, receiver, loop):
    self._receiver = receiver
    self._loop = loop
    super().__init__(loop_sched_operation._execute_impl)

  def start(self):
    self._loop.enqueue(self)

  @staticmethod
  def _execute_impl(t):
    assert isinstance(t, loop_sched_operation)
    _self = t
    _self._receiver.set_value()

class loop_scheduler_task:
  def __init__(self, loop):
    self._loop = loop

  def connect(self, receiver):
    return loop_sched_operation(receiver, self._loop)

class loop_scheduler:
  def __init__(self, loop) -> None:
    self._loop = loop

  def schedule(self):
    return loop_scheduler_task(self._loop)
    
class manual_event_loop: # manual_event_loop in libunifex
  def __init__(self) -> None:
    self._head: typing.Optional[task_base] = None
    self._tail: typing.Optional[task_base] = None
    self._stop = False
    # Mutex and condition variable are ignored in this demo.

  def get_scheduler(self):
    return loop_scheduler(self)

  def run(self):
    while True:
      while self._head is None:
        if self._stop:
          return
      task = self._head
      self._head = task._next
      if self._head is None:
        self._tail = None
      task.execute()

  def stop(self):
    self._stop = True

  def enqueue(self, t):
    if self._head is None:
      self._head = t
    else:
      self._tail._next = t

    self._tail = t
    self._tail._next = None

class single_thread_context:
  def __init__(self) -> None:
    self._loop = manual_event_loop()
    self._thread = threading.Thread(target=lambda: self._loop.run())
    self._thread.start()
    print("ST context launched.")

  def get_scheduler(self):
    return self._loop.get_scheduler()

  def __enter__(self):
    return self

  def __exit__(self, *args):
    self._loop.stop()
    self._thread.join()

  @staticmethod
  def create():
    return single_thread_context()

class then_sender:
  def __init__(self, predecessor, fn):
    self._predcessor = predecessor
    self._fn = fn

  def connect(self, receiver):
    return self._predcessor.connect(
      then_receiver(self._fn, receiver)
    )

class then_receiver:
  def __init__(self, fn, receiver):
    self._fn = fn
    self._receiver = receiver

  def set_value(self, *args, **kwargs):
    result = self._fn(*args, **kwargs)
    assert isinstance(result, va_pack)
    result.invoke(self._receiver.set_value)

def then(sender, fn):
  return then_sender(sender, fn)

class let_value_op:
  def __init__(self, pred, succFact, receiver) -> None:
    self._succFact = succFact
    self._receiver = receiver
    self._predOp = pred.connect(let_value_pred_receiver(self))
    self._values = va_pack

  def start(self):
    self._predOp.start()

class let_value_pred_sender:
  def __init__(self, pred, succFact):
    self._pred = pred
    self._succFact = succFact

  def connect(self, receiver):
    return let_value_op(self._pred, self._succFact, receiver)

class let_value_succ_receiver:
  def __init__(self, op):
    self._op = op

  def set_value(self, *args, **kwargs):
    self._op._receiver.set_value(*args, **kwargs)

class let_value_pred_receiver:
  def __init__(self, op):
    self._op = op

  def set_value(self, *args, **kw_args):
    succOp = self.va_pack.invoke(self._op._succFact).connect(let_value_succ_receiver(self._op))
    succOp.start()

def let_value(pred, succFact):
  return let_value_pred_sender(pred, succFact)


class sync_wait_receiver:
  def __init__(self, ctx):
    self._ctx = ctx

  def set_value(self, *args, **kwargs):
    print(*args, **kwargs)
    self.signal_complete()

  def signal_complete(self):
    self._ctx.stop()

def sync_wait(sender):
  ctx = manual_event_loop()
  op = sender.connect(sync_wait_receiver(ctx))
  print(f"Invoke op.start() where op is <{op.__class__.__name__}>")
  op.start()
  print("Executing sync_wait.ctx.run()")
  ctx.run()
  print("sync_wait completed")


# ... TO REMAKE ...
# Algorithms:
#   let_value
#   just
#   transfer
#   bulk
#   repeat_effect_until
# Contexts:
#   timed_single_thread_context
# ... ... ... ... ...

def testThen():
  with single_thread_context.create() as context:
    scheduler = context.get_scheduler()
    
    count = [0]

    def inc_count():
      count[0] += 1
      return va_pack()

    sync_wait(
      then(
        then(
          scheduler.schedule(), inc_count
        ),
        inc_count
      )
    )
    
    assert count[0] == 2

def testLetOp():
  pass

def _main():
  testThen()

if __name__ == "__main__":
  _main()