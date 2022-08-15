import { Injectable } from '@angular/core';
import { AngularFireDatabase, AngularFireObject, SnapshotAction } from '@angular/fire/database';
import { Observable } from 'rxjs';
import { Parametros } from '../model/parametros';

@Injectable({
  providedIn: 'root'
})
export class ParametrosService {

  parametrosDB:AngularFireObject<Parametros>

  constructor(private db:AngularFireDatabase) {
    this.parametrosDB = this.db.object("/parametros");
   }

   getParametro():Observable<SnapshotAction<Parametros>>
   {
      return this.parametrosDB.snapshotChanges();
   }

   updateParametros(parametros:Parametros)
   {
      this.db.object("/parametros").update(parametros);
   }
}
