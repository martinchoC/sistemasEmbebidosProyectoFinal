import { NgModule } from "@angular/core";
import { SidebarComponent } from './sidebar/sidebar.component';
import { HeaderComponent } from './header/header.component';
import { RouterModule } from '@angular/router';

@NgModule({
  imports:[
    RouterModule
  ],
    declarations:[
        SidebarComponent,
        HeaderComponent,
    ],
    exports:[
        SidebarComponent,
        HeaderComponent,
    ]
})

export class SharedModule{}
