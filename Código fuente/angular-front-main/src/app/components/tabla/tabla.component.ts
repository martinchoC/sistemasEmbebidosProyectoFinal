import { Component, Input, OnInit } from '@angular/core';
import { Data } from 'src/app/model/data';

@Component({
  selector: 'app-tabla',
  templateUrl: './tabla.component.html',
  styleUrls: ['./tabla.component.css']
})
export class TablaComponent implements OnInit {

  @Input("data") dataSnapshot:Data;
  @Input("estadoTittle") estado:string;

  constructor() { }

  ngOnInit(): void {
  }

}
