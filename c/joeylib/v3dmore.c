fillin V3d V3d::qorientate(V3d v,Ori ori) {
  return v.x*ori.x+v.y*ori.y+v.z*ori.qz;
}
fillin V3d V3d::orientate(V3d v,Ori ori) {
  ori.ensureaxesnormalisedbecauseimgoingtoreadthem(); // Steve style =)
  return v.x*ori.x+v.y*ori.y+v.z*ori.z();
}
fillin V3d V3d::disorientate(V3d v,Ori ori) {
  ori.ensureaxesnormalisedbecauseimgoingtoreadthem(); // Steve style =)
  return V3d(V3d::dot(v,ori.x),V3d::dot(v,ori.y),V3d::dot(v,ori.z()));
}

V3d V3d::orientate(Ori ori) {
  return V3d::orientate(*this,ori);
}
V3d V3d::disorientate(Ori ori) {
  return V3d::disorientate(*this,ori);
}
